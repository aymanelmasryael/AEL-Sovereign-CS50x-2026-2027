# AEL CS Encyclopedia™ – Week 7 SQL & AI: Practical Reverse Engineering (AR)

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
  "domain": "Week 7 SQL & Artificial Intelligence: Query Execution Plans & Index Scans",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔓 الهندسة العكسية التطبيقية: تفكيك خطط التنفيذ (EXPLAIN QUERY PLAN)

في الهندسة العكسية لقواعد البيانات، لا نكتفي بمعرفة أن الاستعلام يعمل، بل نسعى لمعرفة *كيف* ينفذ محرك قاعدة البيانات الاستعلام خلف الكواليس، وأي الطرق يسلكها في فحص الجداول.

### 1. تشريح أمر EXPLAIN QUERY PLAN
عند إضافة `EXPLAIN QUERY PLAN` قبل أي جملة `SELECT`، يقوم المحرك بإيقاف التنفيذ الفعلي ويعيد هيكلاً شجرياً يوضح استراتيجية التنفيذ.

```text
===================================================================================
                   EXPLAIN QUERY PLAN REVERSE ENGINE TOPOLOGY
===================================================================================

  [ EXPLAIN QUERY PLAN SELECT * FROM movies WHERE title = 'Inception'; ]
  
  ├── Case A (Unindexed) : SCAN TABLE movies (Worst Case O(N))
  └── Case B (Indexed)   : SEARCH TABLE movies USING COVERING INDEX (Optimal O(log N))

===================================================================================
```

### 2. الفروق الجوهرية في استراتيجيات الفحص
*   **المسح الشامل للجدول (`SCAN TABLE / Seq Scan`)**: يعني أن محرك قاعدة البيانات سيفتح ملف قاعدة البيانات ويقرأ كل صفحة من البداية إلى النهاية. إذا كان الجدول يضم 10 ملايين سجل، سيتم إجراء 10 ملايين فحص.
*   **فحص الفهرس (`SEARCH TABLE USING INDEX / Index Scan`)**: يشير إلى أن المحرك سيتخطى الجدول الفعلي ويتجه مباشرة إلى شجرة `B-Tree` الخاصة بالفهرس، ويقفز للورقة المطابقة في بضع حركات فقط.
*   **الفهرس المغطي (`COVERING INDEX`)**: الحالة الأكثر كفاءة على الإطلاق. تحدث عندما يحتوي الفهرس نفسه على كل الأعمدة المطلوبة في جملة `SELECT`، مما يلغي الحاجة للرجوع إلى الجدول الأصلي تماماً.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
