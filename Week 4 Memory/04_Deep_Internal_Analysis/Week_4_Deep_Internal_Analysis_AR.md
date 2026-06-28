# AEL CS Encyclopedia™ – Week 4 Memory: Deep Internal Analysis (AR)

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
  "domain": "Week 4 Memory: Kernel Level Memory Layout & Sub-Silicon Allocation",
  "document_type": "04_Deep_Internal_Analysis (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 التحليل الباطني العميق: تشريح المكدس والكومة في طبقات النواة (`Kernel Level Layout`)

في التحليل الباطني، نغوص تحت سيطرة المترجم لنراقب كيف تتعامل نواة نظام التشغيل (`The OS Kernel`) مع طلبات الذاكرة في الذاكرة الافتراضية (`Virtual Memory`).

---

## 🛡️ ركائز التحليل الباطني للذاكرة (`Deep Analysis Pillars`)

### 1. تشريح نداءات النظام: `brk` و `sbrk` و `mmap`
* **الميكانيكا الباطنية**: الدالة `malloc()` ليست سحراً؛ إنها مجرد غلاف قياسي (`Standard Library Wrapper`). عندما تطلب مساحة ذاكرة، تقوم `malloc` بإصدار نداءات نظام فعلية للنواة:
  * `sys_brk` / `sbrk`: لتحريك حد الكومة (`Program Break`) إلى أعلى.
  * `sys_mmap`: لتخصيص صفحات مجهولة المصدر (`Anonymous Memory Pages`) للطلبات الكبيرة جداً التي تتجاوز 128 كيلوبايت.

```text
===================================================================================
             MALLOC() KERNEL SYSCALL DELEGATION MATRIX
===================================================================================

  [ USER LAYER ]        :   malloc( 512 );            malloc( 1024 * 1024 );
                                 │                              │
  [ GLIBC WRAPPER ]     :   ( Small Chunk < 128KB )    ( Large Chunk > 128KB )
                                 │                              │
  [ KERNEL SYSCALL ]    :   sys_brk() / sbrk()         sys_mmap() (Anonymous Page)

===================================================================================
```

---

### 2. جداول صفحات الذاكرة الافتراضية (`Virtual Page Tables & MMU`)
* **دور وحدة إدارة الذاكرة (`MMU`)**: العناوين الست عشرية التي تراها في برنامجك (`0x7fff...`) ليست العناوين المادية الفعلية في رقاقة الـ `RAM`. إنها عناوين افتراضية تقوم شريحة الـ `MMU` داخل المعالج بتحويلها لحظياً إلى عناوين فعلية في السيليكون باستخدام جداول الصفحات (`Page Tables`).

---

### 3. تشريح خطأ الإشارة الصامتة (`SIGSEGV - Segmentation Fault`)
* **التشخيص الجنائي**: يحدث خطأ `Segmentation Fault` (الإشارة `SIGSEGV`) عندما يكتشف المعالج محاولة فك مرجعية مؤشر يشير إلى صفحة ذاكرة لا يمتلك العملية أذونات قراءتها أو كتابتها (مثل قراءة عنوان `0x0` أو التعديل في جزء النص التنفيذي `Text Segment`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
