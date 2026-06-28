# Computer Science Notes: Week 1 - C (Core Syntax, Data Types & Flow Control)

---

## 🏛️ تشريح بنية لغة C والملفات المصدرية (Anatomy of C & `src1`)

لغة `C` هي لغة إجرائية قياسية توفر تحكماً مباشراً في الذاكرة والعتاد. في هذا المجلد، نغوص في الأسس الأكاديمية لعلوم الحاسب بالاستناد المباشر إلى مستودع الثروة البرمجية `src1` وملف المحاضرة `lecture1.pdf`.

```text
===================================================================================
                       THE ANATOMY OF A C PROGRAM (hello2.c)
===================================================================================

  #include <cs50.h>    ──> Preprocessor Directive (CS50 Library)
  #include <stdio.h>   ──> Preprocessor Directive (Standard Input/Output)

  int main(void)       ──> Main Entry Point (Returns int 0 on success, takes void)
  {
      string answer = get_string("What's your name? "); ──> Variable Instantiation
      printf("hello, %s\n", answer);                    ──> Formatted Output (%s)
  }

===================================================================================
```

---

## 📦 أنواع البيانات ومحددات التنسيق (Data Types & Format Specifiers)

في لغة C، يجب التصريح الصريح عن نوع كل متغير (`Statically Typed`). يحدد النوع حجم الذاكرة المخصصة وكيفية تفسير الأصفار والآحاد.

### 1. أنواع البيانات الأساسية (`Primitive Types`)
*   `int`: الأعداد الصحيحة (`Integers`). تستخدم عادةً 4 بايت (`32 bits`) وتخزن قيماً تتراوح بين `-2,147,483,648` و `2,147,483,647`. (مستلهم من `calculator0.c`).
*   `char`: رمز واحد (`Character`). يستخدم 1 بايت (`8 bits`) ويتم تخزينه كأرقام وفق جدول `ASCII`. (مستلهم من `agree0.c`).
*   `float`: الأعداد العشرية ذات الدقة الفردية (`Floating-point`). تستخدم 4 بايت. تعاني من القصور الكسري (`Floating-Point Imprecision`). (مستلهم من `calculator2.c`).
*   `double`: الأعداد العشرية ذات الدقة المضاعفة (`Double precision`). تستخدم 8 بايت (`64 bits`) لتوفير دقة أكبر في العمليات الحسابية المعقدة.
*   `bool`: القيم المنطقية (`Boolean` - `true / false`). يتطلب استدعاء مكتبة `<cs50.h>` أو `<stdbool.h>`.
*   `string`: السلاسل النصية. في الواقع هي صفيف من الرموز (`Array of chars`) تنتهي برمز نال (`\0`).

### 2. محددات التنسيق للدوال القياسية (`Format Specifiers for printf`)
| محدد التنسيق | نوع البيانات المخصص | الاستخدام الصريح |
| :--- | :--- | :--- |
| `%i` أو `%d` | `int` | لطباعة الأعداد الصحيحة الصريحة |
| `%c` | `char` | لطباعة الرموز المنفردة |
| `%f` | `float` أو `double` | لطباعة الأعداد العشرية (مثال: `%.2f` لتقريب رقمين عشريين) |
| `%s` | `string` | لطباعة السلاسل النصية |

---

## 🔀 هياكل تدفق التحكم والمنطق (Control Flow & Logic)

يتيح التحكم في تدفق البرنامج اتخاذ القرارات بناءً على الظروف الديناميكية. نستعرض هنا التطبيقات المستلهمة من ملفات `compare` و `agree`.

### 1. الشروط المنطقية (`Conditionals - compare0.c`)
```c
if (x < y)
{
    printf("x is less than y\n");
}
else if (x > y)
{
    printf("x is greater than y\n");
}
else
{
    printf("x is equal to y\n");
}
```

### 2. المعاملات المنطقية (`Logical Operators - agree2.c`)
*   `&&` (AND): يتطلب صحة كلا الشرطين.
*   `||` (OR): يكتفي بصحة شرط واحد على الأقل.
*   `!` (NOT): يعكس الحالة المنطقية.

```c
if (c == 'y' || c == 'Y')
{
    printf("Agreed.\n");
}
```

---

## 🔄 البنى التكرارية والتصميم النمطي (Loops & Modular Design)

التكرار هو أساس الكفاءة الحسابية. بدلاً من تكرار الأكواد، نستخدم الحلقات والدوال المستلهمة من ملفات `cat` و `mario`.

### 1. حلقات التكرار (`Iteration Paradigms - cat15.c & mario5.c`)
*   `while`: التكرار القائم على شرط مستمر. مثالي عندما لا نكون متأكدين من عدد التكرارات مسبقاً.
*   `for`: التكرار المحكوم بعدداد صريح (`Initialization; Condition; Update`).
*   `do-while`: الحلقة الحتمية. تنفذ الكود مرة واحدة على الأقل قبل التحقق من الشرط. **تعتبر المعيار الهندسي الذهبي للتحقق من صحة مدخلات المستخدم (`Input Validation`)**.

```c
// Input Validation using do-while (mario5.c)
int n;
do
{
    n = get_int("Size: ");
}
while (n < 1);
```

### 2. التصميم النمطي ونماذج الدوال (`Prototypes & Custom Functions`)
في `cat15.c`، يشرح مالان قاعدة معمارية كبرى في لغة C: المترجم يقرأ الملف من أعلى إلى أسفل. إذا استدعيت دالة مثل `meow(3)` قبل تعريفها، سيفشل المترجم.
*   **الحل الهندسي (`Function Prototype`):** التصريح عن رأس الدالة أعلى الملف `void meow(int n);`، بينما يتم كتابة تفاصيل تنفيذها أسفل الدالة الرئيسية `main`.

```c
#include <stdio.h>

void meow(int n); // Prototype Declaration

int main(void)
{
    meow(3);
}

void meow(int n) // Internal Implementation
{
    for (int i = 0; i < n; i++)
    {
        printf("meow\n");
    }
}
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
