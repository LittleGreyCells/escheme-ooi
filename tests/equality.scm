
(define (test-equality)
  
  (set! s1 "abc")
  (set! s2 "abc")
  (set! n1 10)
  (set! n2 10)
  (set! f1 10.0)
  (set! f2 10.0)
  
  ;;(assert '(eq? s1 s1))
  ;;(assert '(not (eq? s1 s2)))
  (assert (eqv? s1 s2))
  (assert (equal? s1 s2))
  
  ;;(assert '(not (eq? n1 n2)))
  (assert (eqv? n1 n2))
  (assert (equal? n1 n2))
  
  
  (assert (equal? (string->symbol (symbol->string 'a)) 'a))
  (assert (symbol? (gensym)))
)

