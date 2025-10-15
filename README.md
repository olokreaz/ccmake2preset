# ccmake2preset

A utility for converting plain text descriptions of CMake variables into JSON or command-line interface (CLI) format.

## 🧩 Purpose

Converting CMakeCache entries into a convenient working format:

```
KEY:TYPE=VALUE
```

and transforms this description:

* or to a JSON file (`out.json` by default),
* or into a set of `-D` parameters to pass to CMake via CLI.

This is convenient for generating `CMakePresets.json` or quickly building parameters in scripts.

---

## Build

```bash
git clone https://github.com/olokreaz/ccmake2preset
cd ccmake2preset
make
```

The result will be the executable file `ccmake2preset`.

---

## 📖 Usage

```bash
ccmake2preset <input_file> [output | +/-]
```

### Arguments:

* `<input_file>` — path to the source text file.
* `output` — name of the JSON file (default `out.json`).
* `-` — output CLI-format line by line (`-DKEY:TYPE="VALUE"`).
* `+` — output CLI-format on a single line.

---

## 🧾 Example Input

```txt
FIRST_CACHE_VARIABLE:BOOL=OFF
INSTALL_PATH:PATH=/usr/local
ENABLE_DEBUG:BOOL=ON
# comment
```

---

## 📤 Example Output (JSON)

```bash
./ccmake2preset CMakeCache.txt
```

Result (`out.json`):

```json
{
	"FIRST_CACHE_VARIABLE": { "type": "BOOL", "value": "OFF"  },
	"INSTALL_PATH": { "type": "PATH", "value": "/usr/local"  },
	"ENABLE_DEBUG": { "type": "BOOL", "value": "ON"  }
}
```

---

## 💻 Example output (CLI)

```bash
./ccmake2preset CMakeCache.txt -
```

Result:

```bash
-DFIRST_CACHE_VARIABLE:BOOL="OFF"
-DINSTALL_PATH:PATH="/usr/local"
-DENABLE_DEBUG:BOOL="ON"
```

Or on one line:

```bash
./ccmake2preset CMakeCache.txt +
```

Result:

```bash
-DFIRST_CACHE_VARIABLE:BOOL="OFF" -DINSTALL_PATH:PATH="/usr/local" -DENABLE_DEBUG:BOOL="ON"
```

---

### 📜 License
[MIT License](./LICENSE) © 2025 olokreaz
