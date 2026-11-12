// mini_json.h — a tiny JSON parser (no dependencies, no exceptions), just enough for the Faust
// metadata JSON a genome ships with: objects, arrays, strings, numbers, true/false/null.
// Part of engines/faust-runner (MIT, same licence as the engine).
#pragma once

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

namespace minijson {

struct Value {
  enum Type { Null, Bool, Num, Str, Arr, Obj } type = Null;
  bool b = false;
  double num = 0;
  std::string str;
  std::vector<Value> arr;
  std::map<std::string, Value> obj;

  const Value* find(const char* key) const {
    if (type != Obj) return nullptr;
    auto it = obj.find(key);
    return it == obj.end() ? nullptr : &it->second;
  }
  double getNum(const char* key, double def) const {
    const Value* v = find(key);
    return (v && v->type == Num) ? v->num : def;
  }
  std::string getStr(const char* key, const std::string& def = "") const {
    const Value* v = find(key);
    return (v && v->type == Str) ? v->str : def;
  }
  bool getBool(const char* key, bool def) const {
    const Value* v = find(key);
    return (v && v->type == Bool) ? v->b : def;
  }
};

class Parser {
 public:
  explicit Parser(const std::string& text) : s(text) {}

  bool parse(Value& out) {
    skipWs();
    if (!parseValue(out)) return false;
    skipWs();
    return pos >= s.size();
  }

 private:
  const std::string& s;
  size_t pos = 0;

  void skipWs() {
    while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) ++pos;
  }
  bool parseValue(Value& v) {
    skipWs();
    if (pos >= s.size()) return false;
    char c = s[pos];
    if (c == '{') return parseObj(v);
    if (c == '[') return parseArr(v);
    if (c == '"') {
      v.type = Value::Str;
      return parseStr(v.str);
    }
    if (c == 't' && s.compare(pos, 4, "true") == 0) { v.type = Value::Bool; v.b = true; pos += 4; return true; }
    if (c == 'f' && s.compare(pos, 5, "false") == 0) { v.type = Value::Bool; v.b = false; pos += 5; return true; }
    if (c == 'n' && s.compare(pos, 4, "null") == 0) { v.type = Value::Null; pos += 4; return true; }
    return parseNum(v);
  }
  bool parseNum(Value& v) {
    size_t start = pos;
    if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) ++pos;
    bool any = false;
    while (pos < s.size() && (isdigit((unsigned char)s[pos]) || s[pos] == '.' || s[pos] == 'e' || s[pos] == 'E' ||
                              s[pos] == '+' || s[pos] == '-')) {
      ++pos;
      any = true;
    }
    if (!any) return false;
    v.type = Value::Num;
    v.num = std::strtod(s.c_str() + start, nullptr);
    return true;
  }
  bool parseStr(std::string& out) {
    if (s[pos] != '"') return false;
    ++pos;
    out.clear();
    while (pos < s.size() && s[pos] != '"') {
      char c = s[pos++];
      if (c == '\\' && pos < s.size()) {
        char e = s[pos++];
        switch (e) {
          case 'n': out += '\n'; break;
          case 't': out += '\t'; break;
          case 'r': out += '\r'; break;
          case 'b': out += '\b'; break;
          case 'f': out += '\f'; break;
          case 'u': {
            if (pos + 4 > s.size()) return false;
            unsigned code = 0;
            for (int i = 0; i < 4; ++i) {
              char h = s[pos++];
              code <<= 4;
              if (h >= '0' && h <= '9') code |= (unsigned)(h - '0');
              else if (h >= 'a' && h <= 'f') code |= (unsigned)(h - 'a' + 10);
              else if (h >= 'A' && h <= 'F') code |= (unsigned)(h - 'A' + 10);
              else return false;
            }
            // encode as UTF-8 (BMP only; surrogate pairs not needed for faust metadata)
            if (code < 0x80) out += (char)code;
            else if (code < 0x800) { out += (char)(0xc0 | (code >> 6)); out += (char)(0x80 | (code & 0x3f)); }
            else { out += (char)(0xe0 | (code >> 12)); out += (char)(0x80 | ((code >> 6) & 0x3f)); out += (char)(0x80 | (code & 0x3f)); }
            break;
          }
          default: out += e; break;
        }
      } else {
        out += c;
      }
    }
    if (pos >= s.size()) return false;
    ++pos;  // closing quote
    return true;
  }
  bool parseArr(Value& v) {
    v.type = Value::Arr;
    v.arr.clear();  // a reused Value must not keep items from an earlier parse
    ++pos;  // [
    skipWs();
    if (pos < s.size() && s[pos] == ']') { ++pos; return true; }
    for (;;) {
      Value item;
      if (!parseValue(item)) return false;
      v.arr.push_back(std::move(item));
      skipWs();
      if (pos >= s.size()) return false;
      if (s[pos] == ',') { ++pos; continue; }
      if (s[pos] == ']') { ++pos; return true; }
      return false;
    }
  }
  bool parseObj(Value& v) {
    v.type = Value::Obj;
    v.obj.clear();  // a reused Value must not keep keys from an earlier parse
    ++pos;  // {
    skipWs();
    if (pos < s.size() && s[pos] == '}') { ++pos; return true; }
    for (;;) {
      skipWs();
      std::string key;
      if (!parseStr(key)) return false;
      skipWs();
      if (pos >= s.size() || s[pos] != ':') return false;
      ++pos;
      Value item;
      if (!parseValue(item)) return false;
      v.obj[key] = std::move(item);
      skipWs();
      if (pos >= s.size()) return false;
      if (s[pos] == ',') { ++pos; continue; }
      if (s[pos] == '}') { ++pos; return true; }
      return false;
    }
  }
};

inline bool parse(const std::string& text, Value& out) { return Parser(text).parse(out); }

}  // namespace minijson
