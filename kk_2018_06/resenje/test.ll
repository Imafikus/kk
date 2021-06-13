; ModuleID = 'my_module'
source_filename = "my_module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"0x%x\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %tmp = alloca i32, align 4
  %flag = alloca i32, align 4
  %flag1 = load i32, i32* %flag, align 4
  %0 = icmp eq i32 %flag1, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 14)
  br label %ifcont

else:                                             ; preds = %entry
  %printfCall2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 14)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %flag3 = load i32, i32* %flag, align 4
  %1 = icmp eq i32 %flag3, 0
  br i1 %1, label %then4, label %else6

then4:                                            ; preds = %ifcont
  %printfCall5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 232)
  br label %ifcont8

else6:                                            ; preds = %ifcont
  %printfCall7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 232)
  br label %ifcont8

ifcont8:                                          ; preds = %else6, %then4
  %flag9 = load i32, i32* %flag, align 4
  %2 = icmp eq i32 %flag9, 0
  br i1 %2, label %then10, label %else12

then10:                                           ; preds = %ifcont8
  %printfCall11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 128)
  br label %ifcont14

else12:                                           ; preds = %ifcont8
  %printfCall13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 128)
  br label %ifcont14

ifcont14:                                         ; preds = %else12, %then10
  %flag15 = load i32, i32* %flag, align 4
  %3 = icmp eq i32 %flag15, 0
  br i1 %3, label %then16, label %else18

then16:                                           ; preds = %ifcont14
  %printfCall17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 64)
  br label %ifcont20

else18:                                           ; preds = %ifcont14
  %printfCall19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 64)
  br label %ifcont20

ifcont20:                                         ; preds = %else18, %then16
  store i32 508, i32* %tmp, align 4
  %tmp21 = load i32, i32* %tmp, align 4
  %ortmp = or i32 %tmp21, 2048
  %flag22 = load i32, i32* %flag, align 4
  %4 = icmp eq i32 %flag22, 0
  br i1 %4, label %then23, label %else25

then23:                                           ; preds = %ifcont20
  %printfCall24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %ortmp)
  br label %ifcont27

else25:                                           ; preds = %ifcont20
  %printfCall26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 %ortmp)
  br label %ifcont27

ifcont27:                                         ; preds = %else25, %then23
  store i32 -1, i32* %flag, align 4
  %tmp28 = load i32, i32* %tmp, align 4
  %ortmp29 = or i32 %tmp28, 2048
  %flag30 = load i32, i32* %flag, align 4
  %5 = icmp eq i32 %flag30, 0
  br i1 %5, label %then31, label %else33

then31:                                           ; preds = %ifcont27
  %printfCall32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %ortmp29)
  br label %ifcont35

else33:                                           ; preds = %ifcont27
  %printfCall34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 %ortmp29)
  br label %ifcont35

ifcont35:                                         ; preds = %else33, %then31
  ret i32 0
}
