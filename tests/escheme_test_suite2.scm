;;
;; (load "escheme_test_suite2.scm")
;;
;; escheme extensions regression test suite
;;
;;   exercise all extended functionality introduced by escheme.scm
;    and other modules.
;;

(if #f
    (begin
     (load "./tests/escheme_test_suite2.scm")

     (repeat run-the-test 1)
     (repeat run-the-test 500)

     (perform-timed-run 10)
     (perform-timed-run 100)
     (perform-timed-run 200)
     (perform-timed-run 300)
     (perform-timed-run 400)
     (perform-timed-run 500)
     (perform-timed-run 1000)

     (perform-timed-run 10000)
     (perform-timed-run 20000)
     (perform-timed-run 30000)

     ))

(define (displayln text)
  (display text)
  (newline))

(define (display-done)
  (newline)
  (newline)
  (displayln "==========================================")
  (displayln "==========================================")
  (displayln "                 DONE!                    ")
  (displayln "==========================================")
  (displayln "==========================================")
  (newline)
  (newline))

(define count 0)
(define failures 0)
(define quiet #t)

(define (assert x)
  (if (not x)
      (begin
       (set! failures (1+ failures))
       )))

;; cdr past the 1st n elements and return car of remaining list
(define (list-ref list n)
  (car (list-tail list n)))

;;
;;
;;

(define (test-driver)

  (test-list-functions)
  (test-array-functions)
  (test-modules)
  (test-local-defines)
  
  )

(define (test-list-functions)

  (let (x alist a1 a2)
    
    (set! x nil)
    (set! alist nil)
    (set! a1 nil)
    (set! a2 nil)
    
    (set! x (list 1 2 3))
    
    (assert (equal? (reverse '()) '()))
    (assert (equal? (reverse '(1)) '(1)))
    (assert (equal? (reverse '(1 2 3)) '(3 2 1)))
    
    (assert (equal? (last-pair '()) '()))
    (assert (equal? (last-pair '(1)) '(1)))
    (assert (equal? (last-pair '(1 2 3)) '(3)))
    
    (assert (equal? (list-tail '(1 2 3) 0) '(1 2 3)))
    (assert (equal? (list-tail '(1 2 3) 1) '(2 3)))
    (assert (equal? (list-tail '(1 2 3) 2) '(3)))
    
    (assert (equal? (list-ref '(1 2 3) 0) 1))
    (assert (equal? (list-ref '(1 2 3) 1) 2))
    (assert (equal? (list-ref '(1 2 3) 2) 3))
    
    (assert (equal? (append '() ()) '()))
    (assert (equal? (append '(a) ()) '(a)))
    (assert (equal? (append '(a) '(b)) '(a b)))
    (assert (equal? (append '(a) '(b) '(c)) '(a b c)))
    (assert (equal? (append '(a) '() '(c)) '(a c)))
    
    (assert (equal? (map (lambda (n) n) '()) '()))
    (assert (equal? (map (lambda (n) n) '(1)) '(1)))
    (assert (equal? (map (lambda (n) n) '(1 2 3)) '(1 2 3)))
    
    (set! x nil)
    
    (assert (equal? (for-each (lambda (n) (set! x (cons n x))) '()) '()) )
    
    (assert (equal? x '()))
    
    (let ((push (lambda (n) (set! x (cons n x)))))
      (set! x '())
      (assert (equal? (for-each push '(1)) '()))
      (assert (equal? x '(1)))
      
      (set! x '())
      (assert (equal? (for-each push '(1 2)) '()))
      (assert (equal? x '(2 1)))
      )
    
    (assert (equal? (car '(1 2 3 4 5)) 1))
    (assert (equal? (cdr '(1 2 3 4 5)) '(2 3 4 5)))
    
    (assert (equal? (caar '((1 2 3 4) 5)) 1))
    (assert (equal? (cadr '((1 2 3 4) 5)) 5))
    (assert (equal? (cdar '((1 2 3 4) 5 6)) '(2 3 4)))
    (assert (equal? (cddr '((1 2 3 4) 5 6)) '(6)))
    
    (assert (equal? (caaar '( ((1 2) 3 4) (5 6) 7 8) ) 1))
    (assert (equal? (caadr '( ((1 2) 3 4) (5 6) 7 8) ) 5))
    (assert (equal? (cadar '( ((1 2) 3 4) (5 6) 7 8) ) 3))
    (assert (equal? (caddr '( ((1 2) 3 4) (5 6) 7 8) ) 7))
    (assert (equal? (cdaar '( ((1 2) 3 4) (5 6) 7 8) ) '(2)))
    (assert (equal? (cdadr '( ((1 2) 3 4) (5 6) 7 8) ) '(6)))
    (assert (equal? (cddar '( ((1 2) 3 4) (5 6) 7 8) ) '(4)))
    (assert (equal? (cdddr '( ((1 2) 3 4) (5 6) 7 8) ) '(8)))
    
    (assert (equal? (caaaar '( (((1) 2) 3 4) (5 6) 7 8) ) 1))
    (assert (equal? (caaadr '( ((1 2) 3 4) ((5) 6) 7 8) ) 5))
    (assert (equal? (caadar '( ((1 2) (3) 4) (5 6) 7 8) ) 3))
    (assert (equal? (caaddr '( ((1 2) 3 4) (5 6) (7) 8) ) 7))
    (assert (equal? (cadaar '( ((1 (2)) 3 4) (5 6) 7 8) ) '(2)))
    (assert (equal? (cadadr '( ((1 2) 3 4) (5 (6)) 7 8) ) '(6)))
    (assert (equal? (caddar '( ((1 2) 3 (4)) (5 6) 7 8) ) '(4)))
    (assert (equal? (cadddr '( ((1 2) 3 4) (5 6) 7 (8)) ) '(8)))
    
    (assert (equal? (cdaaar '( (((1 x) 2) 3 4) (5 6) 7 8) ) '(x)))
    (assert (equal? (cdaadr '( ((1 2) 3 4) ((5 x) 6) 7 8) ) '(x)))
    (assert (equal? (cdadar '( ((1 2) (3 x) 4) (5 6) 7 8) ) '(x)))
    (assert (equal? (cdaddr '( ((1 2) 3 4) (5 6) (7 x) 8) ) '(x)))
    (assert (equal? (cddaar '( ((1 (2) x) 3 4) (5 6) 7 8) ) '(x)))
    (assert (equal? (cddadr '( ((1 2) 3 4) (5 (6) x) 7 8) ) '(x)))
    (assert (equal? (cdddar '( ((1 2) 3 (4) x) (5 6) 7 8) ) '(x)))
    (assert (equal? (cddddr '( ((1 2) 3 4) (5 6) 7 (8) x) ) '(x)))
    
    ;;
    ;; element lists
    ;;
    
    (assert (member 'a '(t a s t e)))
    (assert (member 'e '(t a s t e)))
    (assert (memv 'a '(t a s t e)))
    (assert (memq 'a '(t a s t e)))
    
    (assert (member 1 '(1 2 3)))
    (assert (memv 1 '(1 2 3)))
    (assert (not (memq 1 '(1 2 3))))
    
    (assert (member "a" '(1 2 3 "a")))
    (assert (memv "a" '(1 2 3 "a")))
    (assert (not (memq "a" '(1 2 3 "a"))))
    
    ;;
    ;; association lists
    ;;
    
    (set! alist '((a . 1) (b . 2) (c . 3)))
    
    (assert (assoc 'a '((a . 1) (b . 2) (c . 3))) )
    (assert (assoc 'c '((a . 1) (b . 2) (c . 3))) )
    
    (assert (assv 1 '((1 . 1) (2 . 2) (3 . 3))) )
    (assert (assv "a" '(("a" . 1) (b . 2) (c . 3))) )
    
    (assert (assoc 'larry '((larry . PN) (loves . VB) (fran . PN))))
    (assert (assv 'larry '((larry . PN) (loves . VB) (fran . PN))))
    (assert (assq 'larry '((larry . PN) (loves . VB) (fran . PN))))
    
    (assert (assoc '(larry . PN) '(((larry . PN) . 10) ((loves . VB) . 1) ((fran . PN) . 10))))
    (assert (not (assv '(larry . PN) '(((larry . PN) . 10) ((loves . VB) . 1) ((fran . PN) . 10)))) )
    (assert (not (assq '(larry . PN) '(((larry . PN) . 10) ((loves . VB) . 1) ((fran . PN) . 10)))) )
    
    )
  nil
)

(define (test-array-functions)

  (let (x alist a1 a2)
    
    (set! x nil)
    (set! alist nil)
    (set! a1 nil)
    (set! a2 nil)

    ;; single deminsional array
    
    (set! a1 (make-array 2))
    (assert '(equal? a1 '#(() ())) )
    
    (array-set! a1 0 10)
    (assert (equal? a1 '#(10 ())) )
    
    (array-set! a1 1 20)
    (assert (equal? a1 '#(10 20)) )
    
    ;; two dimensional array
    
    (set! a2 (make-array 2 2))
    (assert (equal? a2 '#( #(() ()) #(() ()) )))
    
    (array-set! a2 0 0 10)
    (array-set! a2 1 0 20)
    (assert (equal? a2 '#( #(10 ()) #(20 ()) )))
    )
  nil
  )

(define (test-modules)
  ;; TBI
  )

(define (test-local-defines)
  (define x 10)
  (define y 20)
  (define z 30)
  (assert (equal? (list x y z) '(10 20 30)))
  nil
  )

;;
;; Done
;;

(define (run-the-test)
  (set! count 0)
  (set! failures 0)
  (test-driver)
  (if (not (zero? failures))
      (begin
       (display failures) 
       (displayln " failures")
       (exit))))

(define (repeat f n)
   (while (> n 0)
      (f)
      (set! n (- n 1))
      ))

(define (%sum x sum)
  (if (null? x)
      sum
    (%sum (cdr x) (+ (car x) sum))))

(define (avg x)    
  (let ((n (length x))
	(s (%sum x 0)))
    (/ s (* 1.0 n))))

(define (time-it f)
  (let ((start-time (gettime))
	end-time)
    (f)
    (set! end-time (gettime))
    (let ((secs (- (car end-time) (car start-time)))
	  (nsecs (- (cdr end-time) (cdr start-time)))
	  etime)
      (set! etime (+ (* secs 1000000000) nsecs))
      etime
    )))

(define (timed-test) 
  (time-it (lambda () (repeat run-the-test 50))))

(define (run-n-times f n)
  (let ((results nil)
	(m 0))
    (while (> n 0)
           (if (not quiet)
               (begin
                 (display n)
                 (display " ")
                 (flush-output)))
           (let ((x (f)))
             (set! m (+ m 1))
             (if (< m 100)
                 (set! results (cons x results))))
           (set! n (- n 1)))
    results
    ))

(define (perform-timed-run n)
  (let ((factor 1000000000))
    (let ((x (/ (avg (run-n-times timed-test n)) factor)))
      (display "-- ")
      (display x)
      (display " seconds per iteration")
      (newline)))
  (if (not quiet)
      (begin
        (newline)
        (display "gc: ") (print (gc))
        (display-done)))
  )


