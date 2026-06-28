# AEL CS Encyclopedia™ – Week 3 Algorithms: Practical Reverse Engineering (AR)

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
  "domain": "Week 3: Binary Forensics & Voting Mechanics",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: التحليل الجنائي لملفات `Sort` الغامضة

### 1. فك شفرة `sort1`, `sort2`, `sort3` عبر أداة `time`
في مسألة `Sort`، يُعطى الطالب 3 ملفات تنفيذية غامضة، والمطلوب معرفة أيها يستخدم الترتيب الفقاعي، التحديد، أو الدمج.
* **أسلوب التحقيق الجنائي**: نقوم بتمرير ملفات نصية ضخمة (`random50000.txt`, `sorted50000.txt`, `reversed50000.txt`) ونراقب الأداء عبر سطر الأوامر:
  ```bash
  time ./sort1 sorted50000.txt
  time ./sort2 sorted50000.txt
  time ./sort3 sorted50000.txt
  ```

```text
===================================================================================
             RUNTIME BEHAVIORAL FORENSICS (TIMING ANALYSIS)
===================================================================================

  [ Binary A ] ──> Fast on sorted, VERY slow on reversed ──> BUBBLE SORT (Omega(n))
  [ Binary B ] ──> Slow on sorted, slow on reversed      ──> SELECTION SORT (Theta(n^2))
  [ Binary C ] ──> Blazing fast on ALL inputs            ──> MERGE SORT (O(n log n))

===================================================================================
```

---

## 🧠 المحور الثاني: هندسة أنظمة التصويت (`Plurality & Runoff`)

### 1. تشريح نظام `Plurality` (الأغلبية البسيطة)
* **الفكرة المعمارية**: كل ناخب يختار مرشحاً واحداً فقط. نستخدم مصفوفة من الهياكل `candidates[]`.
* **المنطق البرمجي**: دالة `vote(string name)` تتحقق من وجود الاسم وتزيد عداد الأصوات `candidates[i].votes++`. دالة `print_winner` تبحث عن القيمة العظمى وتطبع كل من يمتلكها (للتعامل مع التعادل `Ties`).

### 2. تشريح نظام `Runoff` (جولات الإعادة الفورية)
* **مصفوفة التفضيلات الثنائية (`preferences[i][j]`)**: تخزن تفضيل الناخب `i` للمرتبة `j`.
* **هندسة الإقصاء (`Elimination Logic`)**: إذا لم يحصل أحد على أكثر من 50%، يتم البحث عن المرشح صاحب أقل عدد من الأصوات وتغيير حالته إلى `eliminated = true`. في الجولة التالية، تنتقل أصوات ناخبيه تلقائياً للمرشح التالي في قائمة تفضيلاتهم.

---

## 🧬 المحور الثالث: التحدي الأعظم في CS50x – تشريح `Tideman`

### 1. مفهوم كوندورسيه (`Condorcet Winner`)
مشروع `Tideman` يُعد التحدي الأشرس في الكورس. يهدف إلى إيجاد المرشح الذي سيفوز في أي مواجهة فردية ضد أي مرشح آخر.

```text
===================================================================================
             TIDEMAN 3-STAGE GRAPH EXECUTION PIPELINE
===================================================================================

  [ TALLY ]  ──> Record pairwise victory margins in pairs[] array
     │
  [ SORT ]   ──> Sort pairs by victory strength (highest margin first)
     │
  [ LOCK ]   ──> Lock edges in locked[][] graph ONLY IF no cycle is created (DFS)

===================================================================================
```

### 2. خوارزمية منع الدوامة (`Cycle Detection - Lock Pairs`)
يكمن الفخ الأكبر في دالة `lock_pairs`. إذا قمنا بإقفال الزوج وجعل `locked[winner][loser] = true`، يجب التأكد من عدم وجود مسار يربط الخاسر بالفائز الأولي (`Cycle`).
* **خوارزمية البحث في العمق (`DFS Recursion`)**: كتابة دالة تكرارية تتتبع المسارات في مصفوفة `locked`. إذا قادنا مسار الخاسر للعودة إلى الفائز، نتجاهل الزوج تماماً لضمان وجود مصدر رسومي (`Source`) يفوز بالانتخابات.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
