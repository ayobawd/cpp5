# CPP05 — Reflections (read this before starting CPP06)

You're done with the evaluation of Module 05. This document is not new
material — it's a second pass over the code you already wrote in `ex00/` and
`ex01/`, looking at it with the question "why is it built exactly this way?"
The goal is to make the ideas stick, and to point at exactly where each one
resurfaces in `../cpp6/`, so the two modules feel like one continuous story
instead of two disconnected assignments.

---

## 1. The Orthodox Canonical Form (OCF) — why four specific functions?

Every class you wrote (`Bureaucrat`, `Form`) declares the same four things,
in the same order:

```cpp
Bureaucrat();                                   // default constructor
Bureaucrat(const std::string& name, int grade); // your "real" constructor
Bureaucrat(const Bureaucrat& copy);              // copy constructor
Bureaucrat& operator=(const Bureaucrat& assign); // copy assignment operator
~Bureaucrat();                                   // destructor
```

That's the Orthodox Canonical Form. It's not a 42-specific invention — it's
the classic C++ answer to a very concrete problem: **once a class manages its
own state, the compiler's default behavior for copying it is not always
correct, so you must be able to say, explicitly, what "make a copy" and
"replace this object's contents" and "clean this object up" mean.**

If you never declare any of these, the compiler silently generates all four
for you (a *memberwise* copy: copy each member one by one). For `Bureaucrat`,
that default would actually work fine — but the whole point of Module 02-09
is training you to declare it **on purpose**, every time, so that the day you
write a class where the default *would* be wrong (say, a class holding a
`char*` it allocated with `new`), doing it right is already a habit, not
something you remember only after a bug.

Two details in your own code make this concrete:

**Why `Bureaucrat::Bureaucrat(const Bureaucrat& copy)` is a separate function
from the assignment operator**, even though they look like they do the same
thing:
- The **copy constructor** runs when a *brand new* object is being born from
  an existing one (`Bureaucrat b2(b1);` or `Bureaucrat b2 = b1;` at
  declaration). There is no "old state" to clean up first.
- **`operator=`** runs on an object that **already exists** and already has a
  valid state that needs to be replaced (`b2 = b1;` where `b2` was already
  alive). That's why `operator=` starts with a self-assignment guard
  (`if (this != &assign)`) — the copy constructor never needs one, because
  there's no "old self" yet to accidentally clobber.

**Why `Bureaucrat::operator=` only ever touches `_grade`:**

```cpp
Bureaucrat& Bureaucrat::operator=(const Bureaucrat& assign) {
    if (this != &assign) {
        this->_grade = assign._grade;
    }
    return *this;
}
```

`_name` is `const std::string`. A `const` member is set exactly once, at
construction, via the initializer list — and never again. This is not a
workaround, it's a *guarantee* the class advertises: "a Bureaucrat's name
never changes after it's created", enforced by the compiler, not by
discipline. `Form`'s assignment operator has the exact same shape for the
exact same reason (`_name`, `_gradeToSign`, `_gradeToExecute` are all
`const`; only `_isSigned` is reassignable).

This is the first appearance of a theme that runs through the whole C++
curriculum: **use the type system to make illegal states impossible to
represent**, instead of trusting yourself to never write the bug. Module 06's
whole premise — that a *cast* should say exactly what kind of conversion you
mean — is the same theme, one level up: instead of trusting yourself to
convert types correctly, the language makes you name your intent
(`static_cast`, `reinterpret_cast`, `dynamic_cast`) so the compiler can check
it.

---

## 2. Exceptions — and why the exception classes break the OCF rule

`Bureaucrat::GradeTooHighException` and `GradeTooLowException` are nested
classes that inherit from `std::exception` and override one function:

```cpp
class GradeTooHighException : public std::exception {
public:
    virtual const char* what() const throw();
};
```

Notice they do **not** follow OCF — no custom copy constructor, no
`operator=`. The subject explicitly exempts exception classes from this rule,
and now you can see why: they carry **no state of their own** (no members),
so the compiler-generated defaults (do nothing, copy nothing) are already
exactly correct. OCF isn't a ritual you perform on every class — it's a
response to *owning state that needs custom handling*. A class with no state
to manage has nothing for OCF to protect.

Three details worth internalizing from how exceptions are actually used:

- **`virtual const char* what() const throw()`** — `virtual` because code
  that catches `std::exception&` (not knowing the concrete exception type)
  still needs to call *your* override, not the base class's; `const` because
  asking "what went wrong?" shouldn't be able to change anything; `throw()`
  is the (now old-fashioned, C++98-era) way of promising "this function
  itself will never throw" — a `what()` that could itself throw while you're
  already handling an exception would be a nightmare to reason about.
- **Constructors throw, not return an error code.** `Bureaucrat`'s
  parameterized constructor has no way to say "construction failed" other
  than throwing — a constructor either fully builds a valid object, or it
  throws and no object exists at all. There's no in-between "half-built
  Bureaucrat" state to worry about elsewhere in your code.
- **`Bureaucrat::signForm` catches by reference, at the boundary where it
  matters:**

  ```cpp
  void Bureaucrat::signForm(Form& form) {
      try {
          form.beSigned(*this);
          std::cout << this->_name << " signed " << form.getName() << std::endl;
      } catch (std::exception& e) {
          std::cout << this->_name << " couldn't sign " << form.getName()
                    << " because " << e.what() << std::endl;
      }
  }
  ```

  `form.beSigned(*this)` can throw `Form::GradeTooLowException`, a type
  `Bureaucrat` doesn't even know exists — and it doesn't need to: catching
  `std::exception&` (the common base) is enough to handle *any* exception
  type that plays by the rules, without `Bureaucrat.cpp` needing to `#include`
  every exception `Form` might ever throw. That's the same idea as `dynamic_cast`
  in `ex02` of Module 06: code written against a **base type** (`std::exception&`,
  `Base&`) can correctly handle objects whose exact concrete type it never
  needed to name.

---

## 3. Encapsulation: private data, public getters, and one deliberate exception

Both classes keep every member `private` and expose only getters:
`getName()`, `getGrade()`, etc. — never a setter for `_name`, because nothing
should ever change a Bureaucrat's name after construction. The *shape* of the
public interface **is** the design: what has a getter but no setter tells you,
at a glance, what's meant to be immutable.

The one function that reaches into another object's private, non-const state
is `Form::beSigned`:

```cpp
void Form::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() <= this->_gradeToSign) {
        this->_isSigned = true;
    } else {
        throw Form::GradeTooLowException();
    }
}
```

It takes the `Bureaucrat` as `const&` — `beSigned` is only allowed to *read*
the bureaucrat's grade, never modify it — while it's allowed to mutate its
**own** `_isSigned`, because that's the one piece of state a `Form` is
supposed to own the changing of. `const` here is doing real, load-bearing
work: it's the difference between "this function may look at you" and "this
function may change you", enforced by the compiler at every call site, not
just in this one function's body.

---

## 4. Why `operator<<` is a free function, not a member

```cpp
std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat);
```

`bob << std::cout` would look wrong, and it's not how `std::cout << bob`
parses either: in `a << b`, `a` (here, `std::ostream`) is the left-hand
operand, and if `operator<<` were a *member* of `Bureaucrat`, `Bureaucrat`
would have to be on the **left**. Since we don't own `std::ostream` (we can't
add a member function to it), the only place this operator can live is as a
free function taking both sides as parameters. This is a standard C++
pattern you'll see again any time you overload a binary operator where "your"
type is naturally the *right*-hand operand.

---

## 5. What Module 05 was quietly building toward

Looking back at the whole design — `Bureaucrat` validating its own grade at
construction and refusing to exist in an invalid state; `Form` refusing to be
signed by an under-ranked `Bureaucrat`; every exception carrying a precise,
readable `what()` — the thread connecting all of it is **explicitness**:
nothing happens implicitly, every risky operation (an out-of-range grade, a
too-low-ranked signature) is checked and named, not silently allowed or
silently ignored.

Module 06 is that exact same philosophy applied to **type conversions**:

- `ScalarConverter` (`../cpp6/ex00/`) refuses to guess — it explicitly
  detects the literal's type before converting it, and explicitly says
  `impossible` rather than printing a nonsense value, the same way
  `Bureaucrat`'s constructor refuses to silently clamp an invalid grade and
  throws instead.
- `Serializer` (`../cpp6/ex01/`) makes the "this is not a normal, safe
  conversion" nature of pointer-to-integer explicit by naming it
  `reinterpret_cast`, instead of hiding it behind a C-style `(uintptr_t)ptr`.
- `identify()` (`../cpp6/ex02/`) is Module 05's `signForm`/`beSigned` pattern
  — code written against a base type, correctly handling a concrete type it
  doesn't statically know — pushed one step further: now the base class
  (`Base`) is genuinely polymorphic (`virtual ~Base()`), and `dynamic_cast`
  asks the object itself what it really is, at runtime, instead of you
  guessing.

If Module 05 made sense to you, Module 06 is not a new topic so much as the
same habit of mind — *say exactly what you mean, let the compiler catch you
when you don't* — pointed at a different corner of the language.
