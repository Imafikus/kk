; ModuleID = 'my module'

%vektor = type { double, double }

@0 = private unnamed_addr constant [10 x i8] c"[%g, %g]\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %v1 = alloca %vektor
  %x_coord = getelementptr %vektor, %vektor* %v1, i32 0, i32 0
  store double 1.100000e+00, double* %x_coord
  %y_coord = getelementptr %vektor, %vektor* %v1, i32 0, i32 1
  store double 2.200000e+00, double* %y_coord
  %x = load double, double* %x_coord
  %y = load double, double* %y_coord
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %x, double %y)
  %printfCall1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double 3.300000e+00, double 4.400000e+00)
  ret i32 0
}
