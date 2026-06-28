# AEL CS Encyclopedia™ – Week 2 Arrays: Projects & Codebases (AR)

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
  "domain": "Week 2: Official Problem Sets & Architecture",
  "document_type": "06_Projects (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: مواصفات وتحديات مشروع سكرابل (`Scrabble Project Specifications`)

### 1. المتطلبات الوظيفية الصارمة (`Strict Functional Requirements`)
مطلوب بناء برنامج تفاعلي بالطرفية يحسب مجموع النقاط لكلمتين يدخلهما مستخدمان منفصلان، ويعلن فوز أحدهما أو التعادل (`Tie!`).

```text
===================================================================================
             SCRABBLE EXECUTION FLOWCHART
===================================================================================

  [ Input P1 ] ──> compute_score() ──┐
                                     ├──> Comparison ──> [ Print: Player X wins! / Tie! ]
  [ Input P2 ] ──> compute_score() ──┘

===================================================================================
```

### 2. معمارية الدالة `compute_score`
يجب أن تتلقى الدالة السلسلة النصية كمعامل `string word` وترجع قيمة صحيحة `int` تمثل مجموع النقاط، مع الالتزام التام بتجاهل أي رموز أو أرقام لا تنتمي للأحرف الأبجدية.

---

## 🧠 المحور الثاني: مواصفات وتحديات مشروع قراءة النصوص (`Readability Specifications`)

### 1. معادلة التقييم الأكاديمي (`Academic Readability Index`)
البرنامج يأخذ نصاً كاملاً (`Text`) من المستخدم ويطبق خوارزمية `Coleman-Liau`:
$$\text{Index} = 0.0588 \times L - 0.296 \times S - 15.8$$

### 2. الشروط الحدية الجنائية (`Edge Cases & Guardrails`)
* **التقريب الرياضي**: استخدام دالة `round()` من مكتبة `<math.h>` للحصول على عدد صحيح.
* **إدارة قسمة الأعداد الصحيحة (`Integer Division Hazard`)**: عند حساب المتوسطات، يجب تحويل المتغيرات صراحةً إلى `float` (مثال: `(float) letters / words`) لتجنب فقدان الكسور العشرية.

---

## 🧬 المحور الثالث: مواصفات مشاريع التشفير (`Caesar & Substitution`)

### 1. شروط مسار التنفيذ لمشروع قيصر (`Caesar Specification`)
* **مفتاح سطر الأوامر**: التأكد من توفر معامل واحد فقط `argc == 2`، وأن يكون مفتاحاً رقمياً صالحاً.
* **إشعار الخطأ القياسي**: في حال إدخال معامل غير صالح، يطبع البرنامج `Usage: ./caesar key` ويرجع القيمة `1` للدلالة على وجود خطأ.

### 2. شروط مسار التنفيذ لمشروع الاستبدال (`Substitution Specification`)
* **هندسة المفتاح**: التأكد من أن المفتاح `argv[1]` مكون من 26 حرفاً أبجدياً فريداً (لا توجد تكرارات سواء كانت كبيرة أو صغيرة).
* **الحفاظ على البنية الأصلية**: الحفاظ الكامل على حالة الحروف (`Case Sensitivity`) ومسافات النص الأصلي.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
