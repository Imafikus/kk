; ModuleID = 'my_module'

%vektor = type { double, double }

@0 = private unnamed_addr constant [10 x i8] c"[%g, %g]\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca %vektor
  %v3 = alloca %vektor
  %v2 = alloca %vektor
  %v1 = alloca %vektor
  %v = alloca %vektor
  store %vektor { double 1.000000e+00, double 3.140000e+00 }, %vektor* %v
  store %vektor { double -2.000000e+00, double 1.000000e+00 }, %vektor* %v1
  store %vektor zeroinitializer, %vektor* %v2
  store %vektor zeroinitializer, %vektor* %v3
  %v4 = load %vektor, %vektor* %v
  %x = extractvalue %vektor %v4, 0
  %y = extractvalue %vektor %v4, 1
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %x, double %y)
  %x_coord = getelementptr %vektor, %vektor* %v2, i32 0, i32 0
  store double 0.000000e+00, double* %x_coord
  %y_coord = getelementptr %vektor, %vektor* %v2, i32 0, i32 1
  store double 1.000000e+00, double* %y_coord
  %v5 = load %vektor, %vektor* %v
  %x6 = extractvalue %vektor %v5, 0
  %y7 = extractvalue %vektor %v5, 1
  %multmp1 = fmul double 3.000000e+00, %x6
  %multmp2 = fmul double 3.000000e+00, %y7
  %v28 = load %vektor, %vektor* %v2
  %x9 = extractvalue %vektor %v28, 0
  %y10 = extractvalue %vektor %v28, 1
  %addtmp1 = fadd double %multmp1, %x9
  %addtmp2 = fadd double %multmp2, %y10
  %printfCall11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %addtmp1, double %addtmp2)
  %v212 = load %vektor, %vektor* %v2
  %x13 = extractvalue %vektor %v212, 0
  %y14 = extractvalue %vektor %v212, 1
  %subtmp1 = fsub double %x13, 1.000000e+01
  %subtmp2 = fsub double %y14, 1.000000e+01
  %x_coord15 = getelementptr %vektor, %vektor* %v3, i32 0, i32 0
  store double %subtmp1, double* %x_coord15
  %y_coord16 = getelementptr %vektor, %vektor* %v3, i32 0, i32 1
  store double %subtmp2, double* %y_coord16
  store %vektor { double 1.000000e+00, double 1.000000e+00 }, %vektor* %c
  %v317 = load %vektor, %vektor* %v3
  %x18 = extractvalue %vektor %v317, 0
  %y19 = extractvalue %vektor %v317, 1
  %printfCall20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %x18, double %y19)
  %c21 = load %vektor, %vektor* %c
  %x22 = extractvalue %vektor %c21, 0
  %y23 = extractvalue %vektor %c21, 1
  %printfCall24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %x22, double %y23)
  ret i32 0
}
