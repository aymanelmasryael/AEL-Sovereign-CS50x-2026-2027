# AEL CS Encyclopedia™ – Week 5 Data Structures: Practical Reverse Engineering (AR)

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
  "domain": "Week 5 Data Structures: Memory Profiling & Pointer Forensic Auditing",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: فحص واكتشاف تسريبات الذاكرة الديناميكية (`Valgrind Memory Profiling`)

### 1. تدقيق تفريغ التتبع (`Valgrind Stack Tracing`)
عند بناء هياكل بيانات ديناميكية كأشجار التريس وجداول التجزئة، يمثل التتبع الجنائي للذاكرة خطوة أساسية لضمان استقرار النظام. عند تشغيل أداة `Valgrind`، يحلل المهندس الجنائي مصفوفات الكتل المفقودة:

```text
===================================================================================
             VALGRIND MEMORY LEAK SUMMARY PROFILE
===================================================================================

  HEAP SUMMARY:
      in use at exit: 2,160 bytes in 10 blocks
    total heap usage: 15 allocs, 5 frees, 3,240 bytes allocated
  LEAK SUMMARY:
      definitely lost: 2,160 bytes in 10 blocks (Unfreed Trie nodes)

===================================================================================
```
تجاوز هذا الخطأ يتطلب بناء دوال هدم غير متزامنة (`Recursive Destructors`) تنظف أبناء العقد قبل تحرير الآباء (`Free children before parents`).

---

## 🧠 المحور الثاني: اكتشاف الحلقات التائهة في القوائم (`Floyd's Cycle Detection`)

### 1. خوارزمية السلحفاة والأرنب (`Tortoise and Hare Algorithm`)
في الهندسة العكسية للقوائم المرتبطة المعطوبة، قد يتسبب مؤشر تالف في إغلاق القائمة على نفسها (`Circular Loop`)، مما يؤدي إلى تجمد النظام بالكامل في حلقة غير منتهية.
يتم كشف هذه الدورة باستخدام مؤشرين: أحدهما يتحرك بسرعة خطوة واحدة (`Slow Pointer`) والآخر بخطوتين (`Fast Pointer`). إذا التقى المؤشران في نقطة ما، يتم تأكيد وجود حلقة مغلقة في الذاكرة.

```text
===================================================================================
             FLOYD'S CYCLE DETECTION MECHANISM
===================================================================================

  [ Head ] ──> [ Node 1 ] ──> [ Node 2 ] ──> [ Node 3 ] ──┐
                                ▲                       │ (Loop Hazard)
                                └───────────────────────┘

===================================================================================
```

---

## 🧬 المحور الثالث: فحص مؤشرات الذاكرة التالفة عبر `GDB`

### 1. تشريح أخطاء التجزئة (`SIGSEGV Dissection`)
عند انهيار البرنامج، يُستخدم المنقح `GDB` مع أمر `backtrace` لفحص السجلات التراكمية للمعالج. يوضح التقرير عنوان الذاكرة الدقيق الذي تسبب في انهيار المؤشر (غالباً محاولة فك مرجعية مؤشر فارغ `Dereferencing NULL pointer` في فتحة تجزئة غير محجوزة).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
