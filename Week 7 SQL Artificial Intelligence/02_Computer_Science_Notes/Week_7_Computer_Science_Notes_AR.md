# AEL CS Encyclopedia™ – Week 7 SQL & AI: Computer Science Notes (AR)

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
  "domain": "Week 7 SQL & Artificial Intelligence: B-Tree Indexing & Mathematical Normalization",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🧠 مذكرات علوم الحاسب: الفهرسة الشجرية والتطبيع الرياضي

تعتبر قواعد البيانات العلائقية تطبيقاً مباشراً لنظرية المجموعات الرياضية (`Set Theory`) والجبر العلائقي (`Relational Algebra`).

### 1. معمارية الفهرسة عبر أشجار B-Tree (`B-Tree Indexing Engine`)
عندما ننفذ أمر `CREATE INDEX` في `SQL`، لا يقوم محرك قاعدة البيانات بمجرد ترتيب البيانات عشوائياً، بل يشيّد بنية شجرية متوازنة ذاتياً تُعرف بـ `B-Tree`.
*   **البحث اللوغاريتمي الفائق**: تضمن شجرة `B-Tree` أن يتساوى عمق كافة الأوراق (`Leaf Nodes`)، مما يجعل عمليات البحث والإدراج والحذف تتم في تعقيد زمني حتمي قدره $O(\log N)$.
*   **تقليل الإدخال والإخراج (`Disk I/O Minimization`)**: تم تصميم عُقد الـ `B-Tree` لتتطابق مع حجم صفحات الذاكرة التخزينية (`Page Size`) في أنظمة التشغيل (غالباً 4KB)، مما يقلل بشكل حاد من عدد مرات القراءة من القرص الصلب.

### 2. الجبر العلائقي وتطبيع الجداول (`Database Normalization`)
يهدف التطبيع (`Normalization`) إلى إزالة التكرار وضمان الاعتمادية المنطقية السليمة:
*   **الصيغة الطبيعية الأولى (`1NF`)**: ضمان أن تكون كل الحقول ذرية (`Atomic Values`) ولا تحتوي على مصفوفات أو جداول متداخلة.
*   **الصيغة الطبيعية الثانية (`2NF`)**: إزالة الاعتماديات الجزئية (`Partial Dependencies`)، بحيث يعتمد كل حقل على المفتاح الأساسي بالكامل.
*   **الصيغة الطبيعية الثالثة (`3NF`)**: إزالة الاعتماديات المتعدية (`Transitive Dependencies`)، بحيث لا يعتمد حقل غير أساسي على حقل غير أساسي آخر.

```text
===================================================================================
                   RELATIONAL ALGEBRA / B-TREE COMPLEXITY
===================================================================================

  [ Flat CSV Search ]        ──> O(N) Worst-case linear disk scans
  [ B-Tree SQL Index ]       ──> O(log N) Self-balancing tree node lookups

===================================================================================
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
