(module
 (table 0 anyfunc)
 (memory $0 1)
 (export "memory" (memory $0))
 (export "x2" (func $x2))
 (export "main" (func $main))
 (func $x2 (; 0 ;) (param $0 i32) (result i32)
  (i32.shl
   (get_local $0)
   (i32.const 1)
  )
 )
 (func $main (; 1 ;) (result i32)
  (call $x2
   (i32.const 3)
  )
 )
)
