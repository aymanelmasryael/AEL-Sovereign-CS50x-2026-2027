# AEL CS Encyclopedia™ – Week 2 Arrays: Computer Science Notes (AR)

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
  "domain": "Week 2: Arrays, Debugging & Cryptography",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: أصل كلمة "Bug" والتشخيص الجنائي للأكواد (`Debugging & Grace Hopper`)

### 1. القصة التاريخية للأدميرال جريس هوبر (`Dr. Grace Hopper & The First Bug`)
تعود تسمية الأخطاء البرمجية باسم `Bug` إلى الأدميرال في البحرية الأمريكية الدكتورة **جريس هوبر (`Grace Hopper`)** وفريقها العامل على جهاز الحاسب العملاق **`Harvard Mark II`**. في أحد الأيام، تعطل الحاسب عن العمل، وعند البحث الجنائي داخل الدوائر الكهربائية (`Circuitry`)، عثر الفريق على حشرة عثة حقيقية (`Moth`) محشورة بين المرحلات الكهربائية (`Relays`). تم تسجيل الملاحظة التاريخية في سجل النظام: *"أول حالة فعلية للعثور على حشرة (Bug)"*.

```text
===================================================================================
             HISTORICAL ARCHITECTURE: THE FIRST BUG (HARVARD MARK II)
===================================================================================

  [ Relay Circuitry ] ──> [ Physical Moth Intercepted ] ──> [ System Short Circuit ]
                                      │
                                      ▼
                      [ Recorded in Logbook: "First actual bug" ]
                                      │
                                      ▼
                      [ Birth of Modern Debugging Paradigm ]

===================================================================================
```

### 2. أنواع الأخطاء البرمجية (`Taxonomy of Errors`)
* **أخطاء الصياغة (`Syntax Errors`)**: أخطاء قاتلة توقف عمل المترجم (`Compiler`) فوراً (مثل نسيان الفاصلة المنقوطة `;` أو عدم إدراج مكتبة `<stdio.h>`). المترجم لا يمتلك أي ذكاء لفهم نيتك المنطقية؛ إنه يتفحص القواعد اللغوية الصارمة فقط.
* **الأخطاء المنطقية (`Logical Errors`)**: ينجح البرنامج في التجميع (`Compilation`)، لكنه ينتج سلوكاً غير مرغوب (مثل طباعة 4 قوالب طوب بدلاً من 3 في حلقة تكرارية `for`). هنا يبرز دور المبرمج الجنائي.

### 3. تقنية الطباعة الجنائية (`Debugging via printf`)
أداة التصحيح الأولى للمبرمج المحترف عندما يغيب الذكاء الاصطناعي (`rubber duck`) هي استدعاء دالة `printf` داخل الحلقات التكرارية لفحص مؤشرات الذاكرة الحية وقيم المتغيرات (مثال: `printf("i is %d\n", i);`).

---

## 🧠 المحور الثاني: النماذج المبدئية للدوال وعجلات التدريب (`Function Prototypes & Training Wheels`)

### 1. مشكلة الترتيب الهرمي للمترجم (`Compiler Pre-Declaration Rule`)
المترجم يقرأ شفرة `C` سطراً بسطر من الأعلى إلى الأسفل. إذا حاولت الدالة `main` استدعاء دالة مساعدة مثل `print_column` قبل الإعلان عنها، سيقوم المترجم بإيقاف التجميع وإصدار خطأ `implicit declaration`.

```text
===================================================================================
             FUNCTION PROTOTYPE PIPELINE
===================================================================================

  [ Line 1: void print_column(int height); ] ──> Prototype informs compiler
  [ Line 3: int main(void) { print_column(3); } ] ──> Safe execution in main
  [ Line 10: void print_column(int height) { ... } ] ──> Actual dynamic implementation

===================================================================================
```
* **الحل المعماري (`Function Prototype`)**: نسخ السطر الأول للدالة وإلصاقه فوق `main` مع إضافة فاصلة منقوطة `;`.

### 2. حقيقة مكتبة CS50 ("عجلات التدريب")
في لغة C الصارمة، لا يوجد نوع بيانات يسمى `string`. مكتبة `CS50.h` هي بمثابة **عجلات تدريب الدراجة (`Training Wheels`)**، صُممت لإخفاء تعقيدات الذاكرة والمؤشرات (`Pointers`) في الأسابيع الأولى، تمهيداً لإزالتها كلياً وكشف حقيقة المؤشر النصي `char *`.

---

## 🧬 المحور الثالث: خوارزميات التشفير وتحليل النصوص (`Cryptography & Reading Levels`)

### 1. تحليل مستويات القراءة (`Reading Levels & Algorithmic Parsing`)
يستعرض مالان تجربة قراءة نصوص مختلفة التدرج (من مقتطفات رياض الأطفال `One Fish, Two Fish` إلى نصوص معقدة من رواية `1984` لجورج أورويل). البرمجة تتيح لنا بناء معادلة رياضية تفكك السلسلة النصية في الذاكرة لفحص طول الجمل، الكلمات، والأحرف لتحديد الصف الدراسي المؤهل لقراءتها (`Coleman-Liau Index`).

### 2. التشفير كعلم وفن (`Cryptography Protocol`)
* **التشفير (`Encryption`)**: عملية تحويل نص عادي (`Plaintext`) إلى نص مبهم (`Ciphertext`) عبر دالة تشفير تعتمد على مفتاح سري (`Key`).
* **فك التشفير (`Decryption`)**: عكس العملية الحسابية للوصول إلى النص الأصلي.
* **الهدف الجنائي**: ضمان سرية المراسلات الطرفية حتى لو اعترض المهاجم قنوات الاتصال الفيزيائية.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
