# Searching with Vectors

Given a search query `A` (where `A` is a string), and a corpus `C`, the goal is to return a pointer
to a record if `A` appears in `C` at the correct field position with high probability.

1. Load a substring `s` of `A` into a register. Repeat `s` in the register until the register is filled.
2. Compare `s` with the input at offsets from `0` to `len(s)`
3. If there are matches, assume we likely match `A`. Else, we definitely do not match `A`.
4. Tune `s` to maximize match probability given `s == input substring.`

See `benchmarks/sparsity-test-twitter/bench.c`.

## Other Ideas

* Neighborhood search - e.g., when we want a value at a specific field, search for value and then check the neighborhood to check the field name.
* Context - what's the minimum amount of context needed for a format to make the actual parsing more efficient (if the value needs to be verified)