// geometric-map.js — browser port of kromosynth-desktop's GenomeGeometryMapper WEIGHTS mode
// (packages/web/src/utils/GenomeGeometryMapper.js), so the test host's x/y "weights" pad does the
// same thing as the main web app's geometric performance pad: cursor position (x,y in [0,1], center
// 0.5) becomes a delta vector (dx,dy in [-1,1]); each connection's weight moves by the dot product
// of that vector with the connection's layout midpoint, scaled by SENSITIVITY, rolled into
// [-3, 3]. Layout is deterministic (string hash of seedPrefix-id), identical to the app's.
// Only the weights path is ported (the app's 'frequencies' mode targets DSP-node frequencies).

const CONFIG = { WEIGHT_RANGE: { min: -3.0, max: 3.0 }, SENSITIVITY: 5.0 };

function hashString(str) {
  let hash = 0;
  for (let i = 0; i < str.length; i++) {
    hash = ((hash << 5) - hash) + str.charCodeAt(i);
    hash = hash & hash;
  }
  return Math.abs(hash);
}

function rollValue(value, min, max) {
  const range = max - min;
  let offset = (value - min) % range;
  if (offset < 0) offset += range;
  return min + offset;
}

function getNetworkLayout(network, seedPrefix) {
  if (!network || !network.nodes) return { nodes: new Map(), connections: [] };
  const nodes = network.nodes.map((n) => (typeof n === 'string' ? JSON.parse(n) : n));
  const connections = (network.connections || []).map((c) => (typeof c === 'string' ? JSON.parse(c) : c));
  const nodePositions = new Map();
  nodes.forEach((node, i) => {
    const isInput = node.name === 'OscillatorNode' || node.name === 'NoteOscillatorNode';
    const isOutput = node.name === 'OutNode' || node.name === 'NetworkOutputNode';
    const hash = hashString(`${seedPrefix}-${node.id || i}`);
    let x, y;
    if (isInput) { x = -0.8; y = (hash % 100) / 50 - 1; }
    else if (isOutput) { x = 0.8; y = (hash % 100) / 50 - 1; }
    else { x = (hash % 200) / 100 - 1; y = ((hash >> 8) % 200) / 100 - 1; }
    nodePositions.set(node.id, { x, y });
  });
  const connectionPositions = connections.map((conn) => {
    const sourceId = conn.sourceNode && conn.sourceNode.id ? conn.sourceNode.id : conn.sourceNode;
    const targetId = conn.targetNode && conn.targetNode.id ? conn.targetNode.id : conn.targetNode;
    const s = nodePositions.get(sourceId);
    const t = nodePositions.get(targetId);
    return s && t ? { x: (s.x + t.x) / 2, y: (s.y + t.y) / 2 } : { x: 0, y: 0 };
  });
  return { nodes: nodePositions, connections: connectionPositions };
}

function adjustNetworkWeights(network, dx, dy, layout, shouldStringify = false) {
  if (!network || !network.connections || !layout || !layout.connections) return;
  network.connections.forEach((conn, index) => {
    if (typeof conn === 'string') {
      try { conn = JSON.parse(conn); network.connections[index] = conn; } catch { return; }
    }
    const connPos = layout.connections[index];
    if (!connPos) return;
    const influence = dx * connPos.x + dy * connPos.y;
    const delta = influence * CONFIG.SENSITIVITY;
    conn.weight = rollValue(conn.weight + delta, CONFIG.WEIGHT_RANGE.min, CONFIG.WEIGHT_RANGE.max);
    if (shouldStringify) network.connections[index] = JSON.stringify(conn);
  });
}

// asNEATPatch in a raw API genome may be a char-split object {"0":"{",...}; normalize to a parsed
// network object for the mapper, and the engine accepts either form back.
function unSplitAsNeat(patch) {
  if (!patch) return null;
  if (typeof patch === 'string') { try { return JSON.parse(patch); } catch { return null; } }
  if (typeof patch === 'object' && !Array.isArray(patch)) {
    const keys = Object.keys(patch);
    if (keys.length && keys.every((k) => /^\d+$/.test(k))) {
      const str = keys.sort((a, b) => Number(a) - Number(b)).map((k) => patch[k]).join('');
      try { return JSON.parse(str); } catch { return null; }
    }
    return patch; // already a network object
  }
  return null;
}

/**
 * Apply the weights pad to a raw genome envelope ({genome:{waveNetwork, asNEATPatch}, ...} or a bare
 * genome object). Returns a NEW envelope (input not mutated). x,y in [0,1], center 0.5.
 */
export function adjustGenomeWeights(rawEnvelope, x, y) {
  const dx = (x - 0.5) * 2;
  const dy = (y - 0.5) * 2;
  const out = JSON.parse(JSON.stringify(rawEnvelope));
  const genome = out.genome ?? out;
  if (Math.abs(dx) < 0.01 && Math.abs(dy) < 0.01) return out;
  if (genome.asNEATPatch) {
    const patch = unSplitAsNeat(genome.asNEATPatch);
    if (patch) {
      adjustNetworkWeights(patch, dx, dy, getNetworkLayout(patch, 'patch'), true);
      genome.asNEATPatch = JSON.stringify(patch);
    }
  }
  if (genome.waveNetwork) {
    const off = genome.waveNetwork.offspring;
    if (off) adjustNetworkWeights(off, dx, dy, getNetworkLayout(off, 'wave'), false);
    else if (genome.waveNetwork.CPPNs) {
      for (const freq of Object.keys(genome.waveNetwork.CPPNs)) {
        const cppn = genome.waveNetwork.CPPNs[freq];
        const target = cppn.offspring ?? cppn;
        adjustNetworkWeights(target, dx, dy, getNetworkLayout(target, `wave-${freq}`), false);
      }
    }
  }
  return out;
}
