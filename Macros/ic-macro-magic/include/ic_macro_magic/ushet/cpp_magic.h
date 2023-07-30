/**
 * This header file contains a library of advanced C Pre-Processor (CPP) macros
 * which implement various useful functions, such as iteration, in the
 * pre-processor.
 *
 * Though the file name (quite validly) labels this as magic, there should be
 * enough documentation in the comments for a reader only casually familiar
 * with the CPP to be able to understand how everything works.
 *
 * The majority of the magic tricks used in this file are based on those
 * described by pfultz2 in his "Cloak" library:
 *
 *    https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 *
 * Major differences are a greater level of detailed explanation in this
 * implementation and also a refusal to include any macros which require a O(N)
 * macro definitions to handle O(N) arguments (with the exception of DEFERn).
 */

#ifndef _IC_M_CPP_MAGIC_H
#define _IC_M_CPP_MAGIC_H

/**
 * Force the pre-processor to expand the macro a large number of times. Usage:
 *
 *   EVAL(expression)
 *
 * This is useful when you have a macro which evaluates to a valid macro
 * expression which is not subsequently expanded in the same pass. A contrived,
 * but easy to understand, example of such a macro follows. Note that though
 * this example is contrived, this behaviour is abused to implement bounded
 * recursion in macros such as FOR.
 *
 *   #define _IC_M_A(x) x+1
 *   #define _IC_M_EMPTY
 *   #define _IC_M_NOT_QUITE_RIGHT(x) A _IC_M_EMPTY (x)
 *   NOT_QUITE_RIGHT(999)
 *
 * Here's what happens inside the C preprocessor:
 *
 * 1. It sees a macro "NOT_QUITE_RIGHT" and performs a single macro expansion
 *    pass on its arguments. Since the argument is "999" and this isn't a macro,
 *    this is a boring step resulting in no change.
 * 2. The NOT_QUITE_RIGHT macro is substituted for its definition giving "A
 *    _IC_M_EMPTY() (x)".
 * 3. The expander moves from left-to-right trying to expand the macro:
 *    The first token, A, cannot be expanded since there are no brackets
 *    immediately following it. The second token _IC_M_EMPTY(), however, can be
 *    expanded (recursively in this manner) and is replaced with "".
 * 4. Expansion continues from the start of the substituted test (which in this
 *    case is just _IC_M_EMPTY), and sees "(999)" but since no macro name is present,
 *    nothing is done. This results in a final expansion of "A (999)".
 *
 * Unfortunately, this doesn't quite meet expectations since you may expect that
 * "A (999)" would have been expanded into "999+1". Unfortunately this requires
 * a second expansion pass but luckily we can force the macro processor to make
 * more passes by abusing the first step of macro expansion: the preprocessor
 * expands arguments in their own pass. If we define a macro which does nothing
 * except produce its arguments e.g.:
 *
 *   #define _IC_M_PASS_THROUGH(...) __VA_ARGS__
 *
 * We can now do "PASS_THROUGH(NOT_QUITE_RIGHT(999))" causing "NOT_QUITE_RIGHT" to be
 * expanded to "A (999)", as described above, when the arguments are expanded.
 * Now when the body of PASS_THROUGH is expanded, "A (999)" gets expanded to
 * "999+1".
 *
 * The EVAL defined below is essentially equivalent to a large nesting of
 * "PASS_THROUGH(PASS_THROUGH(PASS_THROUGH(..." which results in the
 * preprocessor making a large number of expansion passes over the given
 * expression.
 */
#define _IC_M_EVAL(...) _IC_M_EVAL1024(__VA_ARGS__)
#define _IC_M_EVAL1024(...) _IC_M_EVAL512(_IC_M_EVAL512(__VA_ARGS__))
#define _IC_M_EVAL512(...) _IC_M_EVAL256(_IC_M_EVAL256(__VA_ARGS__))
#define _IC_M_EVAL256(...) _IC_M_EVAL128(_IC_M_EVAL128(__VA_ARGS__))
#define _IC_M_EVAL128(...) _IC_M_EVAL64(_IC_M_EVAL64(__VA_ARGS__))
#define _IC_M_EVAL64(...) _IC_M_EVAL32(_IC_M_EVAL32(__VA_ARGS__))
#define _IC_M_EVAL32(...) _IC_M_EVAL16(_IC_M_EVAL16(__VA_ARGS__))
#define _IC_M_EVAL16(...) _IC_M_EVAL8(_IC_M_EVAL8(__VA_ARGS__))
#define _IC_M_EVAL8(...) _IC_M_EVAL4(_IC_M_EVAL4(__VA_ARGS__))
#define _IC_M_EVAL4(...) _IC_M_EVAL2(_IC_M_EVAL2(__VA_ARGS__))
#define _IC_M_EVAL2(...) _IC_M_EVAL1(_IC_M_EVAL1(__VA_ARGS__))
#define _IC_M_EVAL1(...) __VA_ARGS__


/**
 * Macros which expand to common values
 */
#define _IC_M_PASS(...) __VA_ARGS__
#define _IC_M_EMPTY()
#define _IC_M_COMMA() ,
#define _IC_M_PLUS() +
#define _IC_M_ZERO() 0
#define _IC_M_ONE() 1

/**
 * Causes a function-style macro to require an additional pass to be expanded.
 *
 * This is useful, for example, when trying to implement recursion since the
 * recursive step must not be expanded in a single pass as the pre-processor
 * will catch it and prevent it.
 *
 * Usage:
 *
 *   DEFER1(IN_NEXT_PASS)(args, to, the, macro)
 *
 * How it works:
 *
 * 1. When DEFER1 is expanded, first its arguments are expanded which are
 *    simply IN_NEXT_PASS. Since this is a function-style macro and it has no
 *    arguments, nothing will happen.
 * 2. The body of DEFER1 will now be expanded resulting in _IC_M_EMPTY() being
 *    deleted. This results in "IN_NEXT_PASS (args, to, the macro)". Note that
 *    since the macro expander has already passed IN_NEXT_PASS by the time it
 *    expands _IC_M_EMPTY() and so it won't spot that the brackets which remain can be
 *    applied to IN_NEXT_PASS.
 * 3. At this point the macro expansion completes. If one more pass is made,
 *    IN_NEXT_PASS(args, to, the, macro) will be expanded as desired.
 */
#define _IC_M_DEFER1(id) id _IC_M_EMPTY()

/**
 * As with DEFER1 except here n additional passes are required for DEFERn.
 *
 * The mechanism is analogous.
 *
 * Note that there doesn't appear to be a way of combining DEFERn macros in
 * order to achieve exponentially increasing defers e.g. as is done by EVAL.
 */
#define _IC_M_DEFER2(id) id _IC_M_EMPTY _IC_M_EMPTY()()
#define _IC_M_DEFER3(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY()()()
#define _IC_M_DEFER4(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY()()()()
#define _IC_M_DEFER5(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY \
  _IC_M_EMPTY()()()()()
#define _IC_M_DEFER6(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY \
  _IC_M_EMPTY()()()()()()
#define _IC_M_DEFER7(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY \
  _IC_M_EMPTY _IC_M_EMPTY()()()()()()()
#define _IC_M_DEFER8(id) id _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY \
  _IC_M_EMPTY _IC_M_EMPTY _IC_M_EMPTY()()()()()()()()


/**
 * Indirection around the standard ## concatenation operator. This simply
 * ensures that the arguments are expanded (once) before concatenation.
 */
#define _IC_M_CAT(a, ...) a ## __VA_ARGS__
#define _IC_M_CAT3(a, b, ...) a ## b ## __VA_ARGS__


/**
 * Get the first argument and ignore the rest.
 */
#define _IC_M_FIRST(a, ...) a

/**
 * Get the second argument and ignore the rest.
 */
#define _IC_M_SECOND(a, b, ...) b

/**
 * Expects a single input (not containing commas). Returns 1 if the input is
 * PROBE() and otherwise returns 0.
 *
 * This can be useful as the basis of a NOT function.
 *
 * This macro abuses the fact that PROBE() contains a comma while other valid
 * inputs must not.
 */
#define _IC_M_IS_PROBE(...) _IC_M_SECOND(__VA_ARGS__, 0)
#define _IC_M_PROBE() ~, 1


/**
 * Logical negation. 0 is defined as false and everything else as true.
 *
 * When 0, _NOT_0 will be found which evaluates to the PROBE. When 1 (or any other
 * value) is given, an appropriately named macro won't be found and the
 * concatenated string will be produced. IS_PROBE then simply checks to see if
 * the PROBE was returned, cleanly converting the argument into a 1 or 0.
 */
#define _IC_M_NOT(x) _IC_M_IS_PROBE(_IC_M_CAT(_IC_M__NOT_, x))
#define _IC_M__NOT_0 _IC_M_PROBE()

/**
 * Macro version of C's famous "cast to bool" operator (i.e. !!) which takes
 * anything and casts it to 0 if it is 0 and 1 otherwise.
 */
#define _IC_M_BOOL(x) _IC_M_NOT(_IC_M_NOT(x))

/**
 * Logical OR. Simply performs a lookup.
 */
#define _IC_M_OR(a,b) _IC_M_CAT3(_IC__OR_, a, b)
#define _IC_M__OR_00 0
#define _IC_M__OR_01 1
#define _IC_M__OR_10 1
#define _IC_M__OR_11 1

/**
 * Logical AND. Simply performs a lookup.
 */
#define _IC_M_AND(a,b) _IC_M_CAT3(_IC_M__AND_, a, b)
#define _IC_M__AND_00 0
#define _IC_M__AND_01 0
#define _IC_M__AND_10 0
#define _IC_M__AND_11 1


/**
 * Macro if statement. Usage:
 *
 *   IF(c)(expansion when true)
 *
 * Here's how:
 *
 * 1. The preprocessor expands the arguments to _IF casting the condition to '0'
 *    or '1'.
 * 2. The casted condition is concatencated with _IF_ giving _IF_0 or _IF_1.
 * 3. The _IF_0 and _IF_1 macros either returns the argument or doesn't (e.g.
 *    they implement the "choice selection" part of the macro).
 * 4. Note that the "true" clause is in the extra set of brackets; thus these
 *    become the arguments to _IF_0 or _IF_1 and thus a selection is made!
 */
#define _IC_M_IF(c) _IC_M__IF(_IC_M_BOOL(c))
#define _IC_M__IF(c) _IC_M_CAT(_IC_M__IF_,c)
#define _IC_M__IF_0(...)
#define _IC_M__IF_1(...) __VA_ARGS__

/**
 * Macro if/else statement. Usage:
 *
 *   IF_ELSE(c)( \
 *     expansion when true, \
 *     expansion when false \
 *   )
 *
 * The mechanism is analogous to IF.
 */
#define _IC_M_IF_ELSE(c) _IC_M__IF_ELSE(_IC_M_BOOL(c))
#define _IC_M__IF_ELSE(c) _IC_M_CAT(_IC_M__IF_ELSE_,c)
#define _IC_M__IF_ELSE_0(t,f) f
#define _IC_M__IF_ELSE_1(t,f) t


/**
 * Macro which checks if it has any arguments. Returns '0' if there are no
 * arguments, '1' otherwise.
 *
 * Limitation: HAS_ARGS(,1,2,3) returns 0 -- this check essentially only checks
 * that the first argument exists.
 *
 * This macro works as follows:
 *
 * 1. _END_OF_ARGUMENTS_ is concatenated with the first argument.
 * 2. If the first argument is not present then only "_END_OF_ARGUMENTS_" will
 *    remain, otherwise "_END_OF_ARGUMENTS something_here" will remain. This
 *    remaining argument can start with parentheses.
 * 3. In the former case, the _END_OF_ARGUMENTS_(0) macro expands to a
 *    0 when it is expanded. In the latter, a non-zero result remains. If the
 *    first argument started with parentheses these will mostly not contain
 *    only a single 0, but e.g a C cast or some arithmetic operation that will
 *    cause the BOOL in _END_OF_ARGUMENTS_ to be one.
 * 4. BOOL is used to force non-zero results into 1 giving the clean 0 or 1
 *    output required.
 */
#define _IC_M_HAS_ARGS(...) _IC_M_BOOL(_IC_M_FIRST(_IC_M__END_OF_ARGUMENTS_ __VA_ARGS__)(0))
#define _IC_M__END_OF_ARGUMENTS_(...) _IC_M_BOOL(_IC_M_FIRST(__VA_ARGS__))


/**
 * Macro map/list comprehension. Usage:
 *
 *   MAP(op, sep, ...)
 *
 * Produces a 'sep()'-separated list of the result of op(arg) for each arg.
 *
 * Example Usage:
 *
 *   #define MAKE_HAPPY(x) happy_##x
 *   #define COMMA() ,
 *   MAP(MAKE_HAPPY, COMMA, 1,2,3)
 *
 * Which expands to:
 *
 *    happy_1 , happy_2 , happy_3
 *
 * How it works:
 *
 * 1. The MAP macro simply maps the inner MAP_INNER function in an EVAL which
 *    forces it to be expanded a large number of times, thus enabling many steps
 *    of iteration (see step 6).
 * 2. The MAP_INNER macro is substituted for its body.
 * 3. In the body, op(cur_val) is substituted giving the value for this
 *    iteration.
 * 4. The IF macro expands according to whether further iterations are required.
 *    This expansion either produces _IF_0 or _IF_1.
 * 5. Since the IF is followed by a set of brackets containing the "if true"
 *    clause, these become the argument to _IF_0 or _IF_1. At this point, the
 *    macro in the brackets will be expanded giving the separator followed by
 *    _MAP_INNER _IC_M_EMPTY()()(op, sep, __VA_ARGS__).
 * 5. If the IF was not taken, the above will simply be discarded and everything
 *    stops. If the IF is taken, The expression is then processed a second time
 *    yielding "_MAP_INNER()(op, sep, __VA_ARGS__)". Note that this call looks
 *    very similar to the  essentially the same as the original call except the
 *    first argument has been dropped.
 * 6. At this point expansion of MAP_INNER will terminate. However, since we can
 *    force more rounds of expansion using EVAL1. In the argument-expansion pass
 *    of the EVAL1, _MAP_INNER() is expanded to MAP_INNER which is then expanded
 *    using the arguments which follow it as in step 2-5. This is followed by a
 *    second expansion pass as the substitution of EVAL1() is expanded executing
 *    2-5 a second time. This results in up to two iterations occurring. Using
 *    many nested EVAL1 macros, i.e. the very-deeply-nested EVAL macro, will in
 *    this manner produce further iterations, hence the outer MAP macro doing
 *    this for us.
 *
 * Important tricks used:
 *
 * * If we directly produce "MAP_INNER" in an expansion of MAP_INNER, a special
 *   case in the preprocessor will prevent it being expanded in the future, even
 *   if we EVAL.  As a result, the MAP_INNER macro carefully only expands to
 *   something containing "_MAP_INNER()" which requires a further expansion step
 *   to invoke MAP_INNER and thus implementing the recursion.
 * * To prevent _MAP_INNER being expanded within the macro we must first defer its
 *   expansion during its initial pass as an argument to _IF_0 or _IF_1. We must
 *   then defer its expansion a second time as part of the body of the _IF_0. As
 *   a result hence the DEFER2.
 * * _MAP_INNER seemingly gets away with producing itself because it actually only
 *   produces MAP_INNER. It just happens that when _MAP_INNER() is expanded in
 *   this case it is followed by some arguments which get consumed by MAP_INNER
 *   and produce a _MAP_INNER.  As such, the macro expander never marks
 *   _MAP_INNER as expanding to itself and thus it will still be expanded in
 *   future productions of itself.
 */
#define _IC_M_MAP(...) \
   _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))(_IC_M_EVAL(_IC_M_MAP_INNER(__VA_ARGS__)))
#define _IC_M_MAP_INNER(op,sep,cur_val, ...) \
  op(cur_val) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))( \
    sep() _IC_M_DEFER2(_IC_M__MAP_INNER)()(op, sep, ##__VA_ARGS__) \
  )
#define _IC_M__MAP_INNER() _IC_M_MAP_INNER


/**
 * This is a variant of the MAP macro which also includes as an argument to the
 * operation a valid C variable name which is different for each iteration.
 *
 * Usage:
 *   MAP_WITH_ID(op, sep, ...)
 *
 * Where op is a macro op(val, id) which takes a list value and an ID. This ID
 * will simply be a unary number using the digit "I", that is, I, II, III, IIII,
 * and so on.
 *
 * Example:
 *
 *   #define MAKE_STATIC_VAR(type, name) static type name;
 *   _IC_M_MAP_WITH_ID(MAKE_STATIC_VAR, _IC_M_EMPTY, int, int, int, bool, char)
 *
 * Which expands to:
 *
 *   static int I; static int II; static int III; static bool IIII; static char IIIII;
 *
 * The mechanism is analogous to the MAP macro.
 */
#define _IC_M_MAP_WITH_ID(op,sep,...) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))(_IC_M_EVAL(_IC_M_MAP_WITH_ID_INNER(op,sep,I, ##__VA_ARGS__)))
#define _IC_M_MAP_WITH_ID_INNER(op,sep,id,cur_val, ...) \
  op(cur_val,id) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))( \
    sep() _IC_M_DEFER2(_IC_M__MAP_WITH_ID_INNER)()(op, sep, _IC_M_CAT(id,I), ##__VA_ARGS__) \
  )
#define _IC_M__MAP_WITH_ID_INNER() _IC_M_MAP_WITH_ID_INNER


/**
 * This is a variant of the MAP macro which iterates over pairs rather than
 * singletons.
 *
 * Usage:
 *   MAP_PAIRS(op, sep, ...)
 *
 * Where op is a macro op(val_1, val_2) which takes two list values.
 *
 * Example:
 *
 *   #define _IC_M_MAKE_STATIC_VAR(type, name) static type name;
 *   MAP_PAIRS(MAKE_STATIC_VAR, _IC_M_EMPTY, char, my_char, int, my_int)
 *
 * Which expands to:
 *
 *   static char my_char; static int my_int;
 *
 * The mechanism is analogous to the MAP macro.
 */
#define _IC_M_MAP_PAIRS(op,sep,...) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))(_IC_M_EVAL(_IC_M_MAP_PAIRS_INNER(op,sep,__VA_ARGS__)))
#define _IC_M_MAP_PAIRS_INNER(op,sep,cur_val_1, cur_val_2, ...) \
  op(cur_val_1,cur_val_2) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))( \
    sep() _IC_M_DEFER2(_IC_M__MAP_PAIRS_INNER)()(op, sep, __VA_ARGS__) \
  )
#define _IC_M__MAP_PAIRS_INNER() _IC_M_MAP_PAIRS_INNER

/**
 * This is a variant of the MAP macro which iterates over a two-element sliding
 * window.
 *
 * Usage:
 *   MAP_SLIDE(op, last_op, sep, ...)
 *
 * Where op is a macro op(val_1, val_2) which takes the two list values
 * currently in the window. last_op is a macro taking a single value which is
 * called for the last argument.
 *
 * Example:
 *
 *   #define _IC_M_SIMON_SAYS_OP(simon, next) IF(NOT(simon()))(next)
 *   #define _IC_M_SIMON_SAYS_LAST_OP(val) last_but_not_least_##val
 *   #define _IC_M_SIMON_SAYS() 0
 *
 *   MAP_SLIDE(SIMON_SAYS_OP, SIMON_SAYS_LAST_OP, _IC_M_EMPTY, wiggle, SIMON_SAYS, dance, move, SIMON_SAYS, boogie, stop)
 *
 * Which expands to:
 *
 *   dance boogie last_but_not_least_stop
 *
 * The mechanism is analogous to the MAP macro.
 */
#define _IC_M_MAP_SLIDE(op,last_op,sep,...) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))(_IC_M_EVAL(_IC_M_MAP_SLIDE_INNER(op,last_op,sep,__VA_ARGS__)))
#define _IC_M_MAP_SLIDE_INNER(op,last_op,sep,cur_val, ...) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))(op(cur_val,_IC_M_FIRST(__VA_ARGS__))) \
  _IC_M_IF(_IC_M_NOT(_IC_M_HAS_ARGS(__VA_ARGS__)))(last_op(cur_val)) \
  _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__))( \
    sep() _IC_M_DEFER2(_IC_M__MAP_SLIDE_INNER)()(op, last_op, sep, __VA_ARGS__) \
  )
#define _IC_M__MAP_SLIDE_INNER() _IC_M_MAP_SLIDE_INNER


/**
 * Strip any excess commas from a set of arguments.
 */
#define _IC_M_REMOVE_TRAILING_COMMAS(...) \
	_IC_M_MAP(_IC_M_PASS, _IC_M_COMMA, __VA_ARGS__)


#endif
