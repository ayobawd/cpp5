# 42 C++ Modules

C++98 exercises from the 42 C++ Piscine, one folder per module.

| Module | Topic | Status |
|---|---|---|
| [`cpp5/`](cpp5/) | Repetition and Exceptions (Bureaucrat, Form) | Evaluated ✅ — see [`cpp5/REFLECTIONS.md`](cpp5/REFLECTIONS.md) |
| [`cpp6/`](cpp6/) | C++ casts (ScalarConverter, Serializer, real-type identification) | In progress — see [`cpp6/EXPLANATIONS.md`](cpp6/EXPLANATIONS.md) |

Each module folder contains:
- `README.md` — the subject, in the same shape as the official PDF.
- One `exNN/` per exercise, each independently buildable with its own
  `Makefile` (`make`, `make clean`, `make fclean`, `make re`), compiled with
  `-Wall -Wextra -Werror -std=c++98`.
- A deeper write-up (`REFLECTIONS.md` for `cpp5`, `EXPLANATIONS.md` for
  `cpp6`) that explains *why* the code is written the way it is, not just
  what it does — read it alongside the code, not instead of it.

`cpp6/EXPLANATIONS.md` cross-references `cpp5/REFLECTIONS.md` in a few
places: the two modules build on the same ideas (explicit, checked
conversions; ownership and cleanup of `new`'d memory; coding against a base
type without knowing the concrete one), so reading them back to back is
worth the ten minutes.
