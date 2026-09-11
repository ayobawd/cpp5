# CPP06 — Explained in detail

This file walks through *why* every piece of code in this folder is written the
way it is. Read it next to the actual `.hpp`/`.cpp` files. The goal is that by
the end you could re-write any of these three exercises from a blank file,
not just recognize them.

---

## 0. The big idea of this whole module: explicit casts

In C, converting a type is one hammer for every nail:

```c
int   i = (int)some_double;
char *p = (char*)some_pointer;
```

`(type)value` is a **C-style cast**. It quietly picks whichever of "reinterpret
the bits", "convert the value", "strip const", or "walk the class hierarchy"
seems to fit — and the compiler will not stop you if you get it wrong. That's
exactly the kind of hidden bug C++ wants to make visible.

C++ splits that one hammer into four named tools, each doing **one** kind of
conversion, and each refusing to compile (or throwing) if you misuse it:

- **`static_cast<T>(v)`** — converts between related types whose relationship
  the compiler can check at compile time: `int` ↔ `float`, `double` ↔ `char`,
  a derived class pointer up to its base, etc. This is "I know this
  conversion is meaningful, please do it."
- **`reinterpret_cast<T>(v)`** — does *not* convert a value, it relabels the
  same bits as a different, unrelated type. Pointer ↔ integer,
  unrelated pointer ↔ pointer. Dangerous, but exactly what raw memory
  work needs.
- **`dynamic_cast<T>(v)`** — like `static_cast` but for polymorphic class
  hierarchies (classes with at least one `virtual` function), and it is
  **checked while the program runs**: it looks at the object's *actual* type
  and gives you `NULL` (pointer form) or throws `std::bad_cast` (reference
  form) if the cast doesn't make sense for that particular object.
- **`const_cast<T>(v)`** — adds or removes `const`/`volatile`. Not used in
  this module.

The subject's "Additional Rule" asks you to pick **one specific cast per
exercise** and defend that choice. Here's the mapping used in this repo, and
the reasoning for each, in the sections below:

| Exercise | Cast | Why |
|---|---|---|
| `ex00` ScalarConverter | `static_cast` | converting between related numeric types (`double` → `char`/`int`/`float`) |
| `ex01` Serializer | `reinterpret_cast` | relabelling a pointer's bits as an integer, and back |
| `ex02` Identify real type | `dynamic_cast` | discovering an object's real type at runtime, safely |

---

## 1. Exercise 00 — `ScalarConverter`

### What it has to do

Given a string like `"42.0f"` typed on the command line, figure out:
1. What *kind* of literal is this — a `char`, an `int`, a `float`, or a `double`?
2. What is its numeric value?
3. Then print that value explicitly converted to **all four** types, with the
   exact literal-looking formatting C++ uses for each (`42.0f`, not `42`).

### Why one `double` is enough internally

A `char` is just a small integer, an `int` fits inside a `double` exactly (up
to `2^53`), and a `float` is a *less precise* `double`. So instead of writing
four separate parsers, the code parses the input into **one `double` value**,
then prints that same value four times, each time through a different
`static_cast`. That's the whole design in one sentence — everything else in
`ScalarConverter.cpp` is bookkeeping around this idea:

```cpp
double value;
LiteralType type = detectType(literal, value); // fills `value`
printChar(value);    // static_cast<char>(value), with range/printability checks
printInt(value);     // static_cast<int>(value), with range checks
printFloat(value);   // static_cast<float>(value)
printDouble(value);  // value, as-is
```

### `detectType`: reading the literal's *shape*

This function looks only at the *syntax* of the string (not its value yet) to
classify it, in this order — order matters, because later checks assume
earlier ones already failed:

1. **Exact match against the 6 pseudo-literals** (`nanf`, `+inff`, `-inff`,
   `nan`, `+inf`, `-inf`). These aren't parsed, they're just looked up, because
   `strtod` on some platforms doesn't reliably parse `"nan"`/`"inf"` the same
   way everywhere, and we want the exact 6 spellings the subject lists — no
   more, no less.
2. **A single, non-digit character** → `char` literal. `"0"` is *not* a char
   here (it's one character, but it's a digit) — that's why the subject's
   own example (`./convert 0`) treats `0` as an **int**, not the char `'0'`.
3. **No `.` and no `f`** → try to parse the whole string as a number with
   `strtod`. If every character was consumed, it's numeric ("int-shaped").
   We deliberately use `strtod` here, not `strtol`: a string like
   `"2147483648"` is valid *decimal notation* even though it's one too big
   for an `int` — using `strtod` lets `printInt()` say "impossible" for that
   one type later, while `float`/`double` still print the real value, instead
   of throwing away the whole literal.
4. **Ends with `f`** → strip the `f`, require a `.` in what's left, and parse
   it as a `double` with `strtod`. That's a `float` literal (`"4.2f"`).
5. **Anything else with a `.`** → parse the whole thing as a `double`
   (`"4.2"`).
6. If none of that fully consumes the string, it's not a valid literal at all
   → `TYPE_INVALID`, and `convert()` prints `impossible` four times and stops.

`strtod`/`strtol`'s `char** end` output parameter is the key trick used
throughout: after the call, `end` points at the first character *not*
consumed. If `*end == '\0'`, the *entire* string was valid number — that's how
we reject junk like `"12abc"` without writing a manual character-by-character
parser.

### Why NaN/Infinity checks don't use `<cmath>`'s `isnan`/`isinf`

Those functions are C99/C++11 additions; this project targets `-std=c++98`.
Instead the code uses two facts that are guaranteed by the IEEE-754 standard
(and available in C++98 via `<limits>`):

```cpp
bool isNan(double value) { return value != value; }  // NaN is the only value != itself
bool isInf(double value) {
    return value == std::numeric_limits<double>::infinity()
        || value == -std::numeric_limits<double>::infinity();
}
```

### Printing: range checks, then the cast

For `char` and `int`, a value might be perfectly valid as a `double` but make
no sense as that smaller type — too big, too small, or a "special" value
(`NaN`/`Infinity`) that no integer type can represent. So each printer first
rejects those cases, and only then performs the `static_cast`:

```cpp
void printInt(double value) {
    if (isNan(value) || isInf(value) || value < INT_MIN || value > INT_MAX)
        std::cout << "int: impossible" << std::endl;
    else
        std::cout << "int: " << static_cast<int>(value) << std::endl;
}
```

`char` additionally checks *printability* with `std::isprint` after the cast,
because a value can be a perfectly legal `char` (e.g. `0`, the NUL byte) while
still being unprintable — that's the difference between "impossible" (the
value doesn't fit in a `char` at all) and "Non displayable" (it fits, but
there's no visible glyph for it).

`float` and `double` never say "impossible": both types can represent `NaN`
and `Infinity` natively, so the only special-casing they need is *how to spell
it* (`nanf`/`nan`, `+inff`/`+inf`).

### `formatFloating`: making `42.0` not print as `42`

`std::cout << 42.0` prints `42` — correct for a plain number, wrong when we
want to show *"this is a floating-point literal"*. `formatFloating` runs the
value through a normal `<<`, and if the result doesn't already contain a `.`
(and isn't `inf`/`nan`, which must stay as-is), it appends `.0`. This is the
one cosmetic trick in the whole file, and it's the reason `42.0f` and `42.0`
come out looking like literals instead of plain integers.

---

## 2. Exercise 01 — `Serializer`

### What it has to do

Turn a `Data*` pointer into a plain integer (`uintptr_t`) and back, such that
`deserialize(serialize(ptr)) == ptr` always holds.

### Why this is `reinterpret_cast`, not `static_cast`

A pointer and a `uintptr_t` are not "the same value expressed in two related
types" the way an `int` and a `double` are — a pointer's value *is* a memory
address, and `uintptr_t` is defined specifically as *"an unsigned integer type
wide enough to hold that address without losing any bits"*. Converting between
them doesn't compute anything; it just tells the compiler "trust me, treat
this same bit pattern as a number now" / "treat this same bit pattern as an
address again". That relabeling-without-recomputing is precisely what
`reinterpret_cast` exists for — and, fittingly, `static_cast` will straight up
refuse to compile a pointer→integer conversion, because the two types aren't
"related" in the sense `static_cast` requires.

```cpp
uintptr_t Serializer::serialize(Data* ptr) {
    return reinterpret_cast<uintptr_t>(ptr);
}
Data* Serializer::deserialize(uintptr_t raw) {
    return reinterpret_cast<Data*>(raw);
}
```

### Why it round-trips correctly

`serialize` and `deserialize` are exact inverses of each other at the bit
level — no bits are dropped (`uintptr_t` is guaranteed wide enough) and no
bits are recomputed. So for *any* pointer value, converting it to an integer
and immediately back gives you the identical address, which is exactly what
`main.cpp` checks with `&original == restored`.

### Why `Data` needs real members

The subject insists on a **non-empty** struct so the test is meaningful: if
`Data` had no members, comparing addresses wouldn't prove you can still reach
the *object* through the round-tripped pointer, only that the address survived.
`main.cpp` reads `restored->label` and `restored->value` back after the
round trip, to prove the pointer isn't just numerically equal — it's still a
perfectly usable pointer to the same live object.

---

## 3. Exercise 02 — Identify real type

### What it has to do

`generate()` hands back a `Base*` that secretly points at an `A`, `B`, or `C`
object, chosen at random — the caller has no way to know which just by
looking at the static type `Base*`. `identify()` has to figure out the real
type at runtime and print it, once for a pointer and once for a reference,
**without** including `<typeinfo>`.

### Why this needs `dynamic_cast`

`static_cast` is checked entirely at *compile time* — it would let you write
`static_cast<A*>(basePtr)` even when `basePtr` actually points to a `B`,
because the compiler only checks that `A` derives from `Base`, not what the
object actually is at runtime. That would silently produce a pointer that
*lies about its own type* — undefined behavior waiting to happen.

`dynamic_cast` is the one cast that actually asks the object itself
("what are you, really?") while the program is running, using the virtual
table that `Base`'s `virtual ~Base()` guarantees exists. That's why `Base`
*must* have at least one `virtual` function — without it, the class isn't
"polymorphic" and `dynamic_cast` has nothing to inspect at runtime.

### Pointer form: failure is `NULL`

```cpp
void identify(Base* p) {
    if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        ...
}
```

`dynamic_cast<A*>(p)` returns a valid `A*` if `p` really points at an `A` (or
something derived from `A`), and `NULL` otherwise — pointers can always be
"nothing", so a failed cast just becomes "nothing". Trying all three in turn
and checking for a non-`NULL` result is enough to identify the real type.

### Reference form: failure is an exception, and the `<typeinfo>` trap

A `Base&` reference can *never* be null — "no such object" isn't a valid
reference. So `dynamic_cast<A&>(p)` can't fail by returning a special empty
value the way the pointer version does; instead, on failure, it **throws
`std::bad_cast`**.

Here's the catch (literally): `std::bad_cast` is declared in `<typeinfo>`,
and the subject explicitly forbids including that header. The fix is that you
never actually need to *name* the exception type to catch it — `catch (...)`
("catch absolutely anything") works without knowing or including anything
about what might be thrown:

```cpp
void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    } catch (...) {}
    // ... try B, then C ...
}
```

The `(void)` on the cast result silences an "unused value" warning — we don't
need the reference itself, only whether the cast *succeeded without
throwing*.

### Memory: why `main.cpp` deletes what `generate()` returns

`generate()` calls `new A()` / `new B()` / `new C()` and hands ownership to
the caller — nobody else will free that memory. `main.cpp` therefore calls
`delete p` once it's done identifying the object. This only works safely
*because* `~Base()` is `virtual`: deleting a `C` object through a `Base*`
runs `C`'s destructor (trivial here, but it matters in general), not just
`Base`'s. Without `virtual`, deleting through a base pointer to a derived
object is undefined behavior — see `../cpp5/REFLECTIONS.md` for how this
same "always avoid leaks, always think about what `delete` actually runs"
mindset already showed up in Module 05.
