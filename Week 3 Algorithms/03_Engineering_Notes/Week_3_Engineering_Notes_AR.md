# AEL CS Encyclopedia™ – Week 3 Algorithms: Engineering Notes (AR)

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
  "domain": "Week 3: String Comparison & Applied Sorting",
  "document_type": "03_Engineering_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: الفخ الهندسي لمقارنة السلاسل النصية (`The String Comparison Trap`)

### 1. لماذا يفشل العامل `==` مع السلاسل النصية؟
عندما تبحث عن رقم في مصفوفة، يمكنك كتابة `if (numbers[i] == target)`. ولكن إذا كتبت `if (strings[i] == "David")` في لغة `C`، ستحصل على نتيجة كارثية.
* **التشريح الهندسي**: السلسلة النصية هي في الواقع مؤشر لعنوان الذاكرة (`Memory Address`). العامل `==` يقارن العناوين وليس الحروف الفعليه. لذا حتى لو كانت السلسلتان متطابقتين نصياً، فإنهما توجدان في مكانين مختلفين بالذاكرة، وستفشل المقارنة دائماً.

```text
===================================================================================
             STRING EQUALITY MEMORY TRAP
===================================================================================

  if (s == t)  ──>  Compares Address 0x100 with Address 0x200 (Result: FALSE!)
  
  if (strcmp(s, t) == 0) ──> Compares character values 'D','A','V','I','D' (Result: TRUE!)

===================================================================================
```

### 2. مكتبة `<string.h>` ودالة `strcmp`
لإجراء مقارنة فعلية للحروف، نستخدم دالة `strcmp` التي ترجع القيمة `0` إذا تطابقت السلسلتان تماماً:
```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string names[] = {"Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(names[i], "Ron") == 0)
        {
            printf("Found\n");
            return 0; // Exit success
        }
    }
    printf("Not found\n");
    return 1; // Exit failure
}
```

---

## 🧠 المحور الثاني: التطبيق العملي لـ `typedef struct`

### 1. الإعلان عن الهيكل في النطاق العام (`Global Scope`)
لضمان تعرف كافة الدوال على الهيكل المخصص، يتم تعريفه دائماً خارج دالة `main` في أعلى الملف:
```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    string name;
    string number;
}
person;

int main(void)
{
    person people[2];

    people[0].name = "Carter";
    people[0].number = "+1-617-495-1000";

    people[1].name = "David";
    people[1].number = "+1-949-468-2750";

    for (int i = 0; i < 2; i++)
    {
        if (strcmp(people[i].name, "David") == 0)
        {
            printf("Found %s\n", people[i].number);
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}
```
* **تدوين النقطة (`Dot Notation`)**: نستخدم النقطة `.name` أو `.number` للوصول إلى المتغيرات الداخلية داخل كائن الـ `struct`.

---

## 🧬 المحور الثالث: هندسة تحسين الترتيب الفقاعي (`Bubble Sort Optimization`)

### 1. إضافة متغير المراقبة `swapped`
في النسخة التقليدية من الترتيب الفقاعي، يستمر البرنامج في فحص المصفوفة حتى لو أصبحت مرتبة بالكامل. لتحسين الأداء إلى $\Omega(n)$، نضيف متغيراً منطقياً `bool swapped`:

```c
bool swapped;
do
{
    swapped = false;
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            swapped = true;
        }
    }
}
while (swapped);
```
إذا انتهت دورة كاملة دون إجراء أي تبديل (`swapped == false`)، يتوقف البرنامج فوراً، مما يوفر آلاف الدورات المعالجة (`CPU Cycles`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
