# AEL CS Encyclopedia™ – Week 4 Memory: Computer Science Notes (AR)

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
  "domain": "Week 4 Memory: Theoretical Foundations & Memory Math",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 الأصول النظرية: جبر المؤشرات ومحاذاة الذاكرة (`Pointer Algebra & Alignment`)

في علوم الحاسب النظرية، لا يُنظر إلى الذاكرة كمتغيرات عشوائية، بل كـ **فضاء شعاعي لعناوين الذاكرة (`Memory Address Space`)**. يرتكز الأسبوع الرابع على النماذج الرياضية التي تحكم كيفية تنقل المعالج بين هذه الخلايا باستخدام الجبر الست عشري.

---

## 🛡️ ركائز علوم الحاسب النظرية للذاكرة (`Theoretical Pillars`)

### 1. جبر المؤشرات (`Pointer Arithmetic`)
* **القانون الرياضي**: عندما نجمع رقماً $K$ مع مؤشر $P$ من نوع $T$ (`T *P`)، فإن المعالج لا يجمع $K$ بايت، بل يجمع $K \times \text{sizeof}(T)$.
* **مثال تطبيقي**: إذا كان `int *p = 0x1000`، فإن العملية `p + 1` تنقل العنوان إلى `0x1004` (لأن حجم الـ `int` هو 4 بايت).

```text
===================================================================================
             POINTER ARITHMETIC MULTIPLICATION MATRIX
===================================================================================

  [ Base Address: 0x1000 ] (int *p)
   │
   ├─► p + 1 ──> 0x1000 + (1 * 4 bytes) ──> 0x1004
   ├─► p + 2 ──> 0x1000 + (2 * 4 bytes) ──> 0x1008
   └─► p + 3 ──> 0x1000 + (3 * 4 bytes) ──> 0x100C

===================================================================================
```

---

### 2. محاذاة الذاكرة التلقائية (`Memory Alignment & Padding`)
* **الأساس النظري**: المعالجات الحديثة ذات 64-بت تقرأ الذاكرة في كتل بحجم 8 بايت (`Word Size`). إذا تم تعريف متغير بحجم 1 بايت تلاه متغير بحجم 4 بايت، يقوم المترجم بإدخال بايتات فارغة (`Padding Bytes`) لضمان محاذاة الكتل وتسريع الوصول للذاكرة.

---

### 3. التصادم الحتمي: طفح المكدس (`Stack Overflow vs. Buffer Overflow`)
* **`Stack Overflow`**: يحدث عندما تستهلك الاستدعاءات الذاتية (`Infinite Recursion`) كامل المساحة المخصصة للمكدس، مما يؤدي لاصطدامه بحافة الكومة (`Heap`).
* **`Buffer Overflow`**: يحدث عند كتابة بيانات تتجاوز طول المصفوفة المخصصة، مما يؤدي لتدمير عنوان العودة (`Return Address`) في المعالج واختراق النظام.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
