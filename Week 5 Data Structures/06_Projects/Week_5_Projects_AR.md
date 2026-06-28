# AEL CS Encyclopedia™ – Week 5 Data Structures: Projects Overview (AR)

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
  "domain": "Week 5 Data Structures: Speller Dictionary System & Hybrid ADTs",
  "document_type": "06_Projects (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 نظرة عامة على مشروع المدقق الإملائي (`Speller Project Overview`)

يعد مشروع المدقق الإملائي (`Speller`) أحد أشهر وأقوى المشاريع في تاريخ هارفارد `CS50`. يتمثل التحدي في بناء محرك فائق السرعة لقراءة قاموس ضخم يحتوي على أكثر من `140,000` كلمة، ثم فحص نصوص ضخمة للتحقق من صحتها الإملائية في أجزاء من الثانية.

---

## 🛡️ المعمارية الوظيفية الصارمة (`Strict Modular Contracts`)

### 1. دالة التحميل `load`
تقوم بفتح ملف القاموس، وقراءة الكلمات حرفاً بحرف أو سلسلة بسلسلة، وإدخالها في بنية بيانات مناسبة (جدول تجزئة `Hash Table` أو شجرة تريس `Trie`).

```text
===================================================================================
             SPELLER PIPELINE ARCHITECTURE
===================================================================================

  [ Dictionary File ] ──> load() ──> [ Hash Table / Trie ] ──> check() <── [ Text Payload ]
                                              │
                                           unload() ──> [ Memory Completely Freed ]

===================================================================================
```

### 2. دالة التجزئة `hash`
يجب أن تأخذ السلسلة النصية وترجع فهرساً حتمياً `Index` يمثل الموقع الدقيق في جدول التجزئة. الكفاءة هنا تحدد سرعة النظام بأكمله.

### 3. دالة الفحص `check`
تبحث عن الكلمة في بنية البيانات. يجب أن تكون غير حساسة لحالة الأحرف (`Case-Insensitive`)، مما يعني أن `Apple` و `apple` و `APPLE` تعامل بنفس النتيجة.

### 4. دالة التنظيف `unload`
تقوم بالمرور على جميع فتحات جدول التجزئة أو فروع شجرة التريس وتحرير كامل الذاكرة الديناميكية (`free`) لضمان عدم بقاء أي تسريب عبر `Valgrind`.

---

## 🧬 ترسانة الشفرات الحصرية `ael_src5`

استجابةً للأمر السيادي: **"المشاريع أصلية 100% وليست نسخاً من ملفات المصدر الأصلية"**، قمنا بإنشاء 3 شفرات برمجية مؤسسية محفورة في مجلد `ael_src5`:
1.  **`ael_dictionary_chaining.c`**: تطبيق متقدم لنظام المدقق الإملائي بالكامل باستخدام جدول تجزئة مع السلسلة المنفصلة (`Chaining`) وتنظيف مثالي للذاكرة.
2.  **`ael_trie_autocompleter.c`**: محرك قوي لشجرة التريس يتيح فحص الكلمات واسترجاعها بسرعة $O(1)$.
3.  **`ael_binary_search_tree.c`**: شجرة بحث ثنائية متزنة تنفذ أوامر الإدراج، والبحث، والهدم غير المتزامن.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
