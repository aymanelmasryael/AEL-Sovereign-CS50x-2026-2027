# AEL CS Encyclopedia™ – Week 2 Arrays: Engineering Notes (AR)

---

## 🏛️ الهوية السيادية للموسوعة (`AEL Sovereign Identity Block`)

```json
{
  "ael_seal": "AEL CS Encyclopedia — © Ayman Elmasry",
  "owner": "Ayman Elmasry",
  "legal_entities": [
    "Ayman Elmasry LLC (UAE)",
    "Ayman Elmasry Advertising & Marketing (Egypt)"
  ],
  "syllabus_source": "Harvard CS50x 2026-2027",
  "domain": "Week 2: Memory Buffers & Applied Debugging",
  "document_type": "03_Engineering_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: هندسة إصلاح الأخطاء الصارمة (`Rigorous Bug Fixing Mechanics`)

خلال تجربة بناء ملف `buggy.c`، يقدم ديفيد مالان دليلاً هندسياً شاملاً للتعامل مع المترجم (`Compiler`). لا يجب أن يرتعب مهندس البرمجيات من السطور الحمراء؛ بل يتعين عليه تشريحها استناداً إلى أرقام السطور والرموز التشغيلية.

```text
===================================================================================
             CLANG ERROR FORENSICS & RESOLUTION MATRIX
===================================================================================

  [ Error: "Call to undeclared function printf" ] ──> Solution: #include <stdio.h>
  [ Error: "Expected semicolon after expression" ] ──> Solution: Append ';' to line
  [ Error: "Use of undeclared identifier name" ]   ──> Solution: Declare variable type (string)
  [ Error: "Use of undeclared identifier string" ] ──> Solution: #include <cs50.h>

===================================================================================
```

### 1. الخطأ الشائع: `stdio.h` مقابل `studio.h`
من الأسئلة المتكررة سنوياً (`Annual FAQ`) كتابة المبرمجين لكلمة `studio.h` بدلاً من `stdio.h`.
* **التشريح الهندسي**: الكلمة الصحيحة هي اختصار لـ `Standard Input Output`. لا علاقة لها بالاستوديوهات الفنية؛ إنها مكتبة الإدخال والإخراج المعيارية لنظام التشغيل.

### 2. التصريح عن أنواع البيانات (`Explicit Type Declaration`)
على عكس لغات مثل `Python` التي تنشئ المتغيرات ديناميكياً، تتطلب لغة `C` الصارمة حجز المساحة الذاكرية الصريحة مسبقاً:
```c
// Requires <cs50.h> for 'string' definition
string name = get_string("What's your name? ");
printf("hello, %s\n", name);
```

---

## 🧠 المحور الثاني: التحكم في حدود التكرار والنماذج المبدئية (`Loop Bounds & Prototypes`)

### 1. تشريح الخطأ المنطقي في طباعة قوالب الطوب (`The 4 vs 3 Bricks Bug`)
عندما كتب مالان الكود التالي لطباعة 3 قوالب طوب:
```c
for (int i = 0; i <= 3; i++)
{
    printf("#\n");
}
```
أنتج البرنامج 4 قوالب (`i = 0, 1, 2, 3`).

```text
===================================================================================
             LOOP BOUNDARY MEMORY COMPARISON
===================================================================================

  i <= 3 (Incorrect Logic):
  [ i=0 (Brick 1) ] ──> [ i=1 (Brick 2) ] ──> [ i=2 (Brick 3) ] ──> [ i=3 (Brick 4!) ]

  i < 3 (Correct Canonical Logic):
  [ i=0 (Brick 1) ] ──> [ i=1 (Brick 2) ] ──> [ i=2 (Brick 3) ] ──> [ Exit Loop ]

===================================================================================
```

* **القاعدة الهندسية الذهبية**: عند البدء بالفهرس `0`، نستخدم دائماً العامل `<` وليس `<=`.

### 2. النماذج المبدئية المعمارية (`Function Prototypes`)
لكتابة كود نظيف معمارياً، يتم وضع دالة `main` في أعلى الملف، ولكن لضمان عدم اعتراض المترجم على الدوال المساعدة التي تأتي لاحقاً (مثل `print_column`)، نضع نموذج الدالة في السطر الأول:
```c
#include <cs50.h>
#include <stdio.h>

void print_column(int height); // Function Prototype

int main(void)
{
    int h = get_int("Height: ");
    print_column(h);
}

void print_column(int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("#\n");
    }
}
```

---

## 🧬 المحور الثالث: اختبارات التحقق المؤتمتة (`Automated Unit Verification`)

### 1. منصة `check50`
يوضح مالان أن المترجم (`Compiler`) يتأكد فقط من سلامة قواعد اللغة الصارمة (`Syntax`)، لكنه أعمى تماماً عن صحة حلك المنطقي.
* **الآلية الهندسية**: برنامج `check50` هو محرك اختبارات مؤتمت (`Automated Unit Tester`) يقوم بمحاكاة مدخلات المستخدم والتحقق من صحة المخرجات الناتجة بدقة متناهية تشمل حتى المسافات والسطور الجديدة `\n`.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
