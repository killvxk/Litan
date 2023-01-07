files=(
	"test/main.ltn"
	"test/sphinx/sphinx.ltn" 

	"test/var.ltn"
	"test/assign.ltn"
	"test/literals.ltn"
	"test/if_else.ltn"
	"test/switch.ltn"
	"test/choose.ltn"
	"test/for.ltn"
	"test/while.ltn"
	"test/throw.ltn"
	"test/lambda.ltn"
	"test/map.ltn"
	"test/template.ltn"
	"test/reflect_location.ltn"
	"test/reflect_function.ltn"
	"test/reflect_namespace.ltn"
	"test/reflect_expr.ltn"
	"test/iife.ltn"
	"test/function_pointers.ltn"

	"test/static_cast.ltn"
	"test/dynamic_cast.ltn"
	"test/static_copy.ltn"
	
	"test/op_arith_add.ltn"
	"test/op_arith_sub.ltn"
	"test/op_arith_mlt.ltn"
	"test/op_arith_div.ltn"
	"test/op_arith_mod.ltn"
	"test/op_arith_pow.ltn"
	"test/op_neg.ltn"
	"test/op_bit_not.ltn"
	"test/op_bit_and.ltn"
	"test/op_bit_or.ltn"
	"test/op_bit_xor.ltn"
	"test/op_bit_shift_l.ltn"
	"test/op_bit_shift_r.ltn"
	"test/op_logic_not.ltn"
	"test/op_logic_and.ltn"
	"test/op_logic_or.ltn"
	"test/op_comp_eql.ltn"
	"test/op_comp_ueql.ltn"
	"test/op_comp_less.ltn"
	"test/op_comp_less_eql.ltn"
	"test/op_comp_greater.ltn"
	"test/op_comp_greater_eql.ltn"
	"test/op_comp_spaceship.ltn"
	"test/op_deref.ltn"
	"test/op_elvis.ltn"
	"test/op_index.ltn"
	"test/op_nullco.ltn"
	"test/op_nulltest.ltn"
	"test/op_ternary.ltn"

	"test/std/arity.ltn"
	"test/std/empty.ltn"
	"test/std/size.ltn"
	"test/std/bits_get.ltn"
	"test/std/bits_set.ltn"
	"test/std/is_type.ltn"
	"test/std/pi.ltn"
	"test/std/min.ltn"
	"test/std/max.ltn"
	"test/std/clamp.ltn"
	"test/std/round.ltn"
	"test/std/floor.ltn"
	"test/std/ceil.ltn"
	"test/std/abs.ltn"
	"test/std/sin.ltn"
	"test/std/cos.ltn"
	"test/std/tan.ltn"
	"test/std/sqrt.ltn"
	"test/std/hypot.ltn"
	"test/std/log.ltn"
	"test/std/ln.ltn"
	"test/std/ld.ltn"
	"test/std/lg.ltn"
	"test/std/split.ltn"
	"test/std/join.ltn"
	"test/std/replace.ltn"
)

bin_file="tmp/bin"
asm_file="tmp/asm"

rm $bin_file
rm $asm_file

./bin/ltnc -o --asm $asm_file --exe $bin_file --src ${files[*]}
./bin/ltnvm --exe $bin_file --args Hello World 123