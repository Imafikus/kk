; ModuleID = 'my module'
source_filename = "my module"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"0x%x\0A\00", align 1

define i32 @main() {
entry:
  %tmp = alloca i32, align 4
  %flag = alloca i32, align 4

entry1:                                           ; No predecessors!
  store i32 0, i32* %flag, align 4
  %flag2 = load i32, i32* %flag, align 4
  %0 = icmp eq i32 %flag2, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry1
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 14)
  br label %ifcont

else:                                             ; preds = %entry1
  %printfCall3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 14)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %flag4 = load i32, i32* %flag, align 4
  %1 = icmp eq i32 %flag4, 0
  br i1 %1, label %then5, label %else6

then5:                                            ; preds = %ifcont
  %printfCall8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 232)
  br label %ifcont7

else6:                                            ; preds = %ifcont
  %printfCall9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 232)
  br label %ifcont7

ifcont7:                                          ; preds = %else6, %then5
  %flag10 = load i32, i32* %flag, align 4
  %2 = icmp eq i32 %flag10, 0
  br i1 %2, label %then11, label %else12

then11:                                           ; preds = %ifcont7
  %printfCall14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 128)
  br label %ifcont13

else12:                                           ; preds = %ifcont7
  %printfCall15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 128)
  br label %ifcont13

ifcont13:                                         ; preds = %else12, %then11
  %flag16 = load i32, i32* %flag, align 4
  %3 = icmp eq i32 %flag16, 0
  br i1 %3, label %then17, label %else18

then17:                                           ; preds = %ifcont13
  %printfCall20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 64)
  br label %ifcont19

else18:                                           ; preds = %ifcont13
  %printfCall21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 64)
  br label %ifcont19

ifcont19:                                         ; preds = %else18, %then17
  store i32 508, i32* %tmp, align 4
  %tmp22 = load i32, i32* %tmp, align 4
  %ortmp = or i32 %tmp22, 2048
  %flag23 = load i32, i32* %flag, align 4
  %4 = icmp eq i32 %flag23, 0
  br i1 %4, label %then24, label %else25

then24:                                           ; preds = %ifcont19
  %printfCall27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %ortmp)
  br label %ifcont26

else25:                                           ; preds = %ifcont19
  %printfCall28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 %ortmp)
  br label %ifcont26

ifcont26:                                         ; preds = %else25, %then24
  store i32 -1, i32* %flag, align 4
  %tmp29 = load i32, i32* %tmp, align 4
  %ortmp30 = or i32 %tmp29, 2048
  %flag31 = load i32, i32* %flag, align 4
  %5 = icmp eq i32 %flag31, 0
  br i1 %5, label %then32, label %else33

then32:                                           ; preds = %ifcont26
  %printfCall35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %ortmp30)
  br label %ifcont34

else33:                                           ; preds = %ifcont26
  %printfCall36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i32 %ortmp30)
  br label %ifcont34

ifcont34:                                         ; preds = %else33, %then32
  ret i32 %ortmp30
  ret i32 0
}

declare i32 @printf(i8*, ...)
