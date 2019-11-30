
(define (repeat f n)
   (while (> n 0)
      (f)
      (set! n (- n 1))))

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
  (time-it (lambda () (repeat run-the-test 10))))

(define (run-n-times f n)
  (let ((results nil)
	(m 0))
    (while (> n 0)
           (if (not quiet)
               (begin
                 (display n)
                 (display " ")
                 (flush-output *standard-output*)
                 ))
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
        (display-done)
        ))
  )

;; [EOF]





