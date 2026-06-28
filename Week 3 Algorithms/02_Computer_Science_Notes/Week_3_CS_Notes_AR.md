# AEL CS Encyclopedia™ – Week 3 Algorithms: Computer Science Notes (AR)

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
  "domain": "Week 3: Data Structs & Computational Theory",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: تجميع البيانات المخصصة وهندسة الـ `Structs`

### 1. القصور المعماري للمصفوفات المتوازية (`The Parallel Arrays Limitation`)
في بداية الأسبوع، عندما حاولنا تخزين أسماء وأرقام هواتف، اضطررنا لإنشاء مصفوفتين منفصلتين (`names[]` و `numbers[]`).
* **الخلل الهندسي**: لا يوجد رابط فيزيائي في الذاكرة بين `names[0]` و `numbers[0]`. إذا تم ترتيب مصفوفة الأسماء دون ترتيب الأرقام بنفس الطريقة، ستتحطم بيانات دليل الهاتف بالكامل.

### 2. تعريف الهياكل المخصصة (`typedef struct`)
لحل هذه المشكلة، توفر لغة `C` آلية قوية لتعريف أنواع بيانات جديدة تجمع عدة متغيرات في كائن واحد:
```c
typedef struct
{
    string name;
    string number;
}
person;

// Memory instantiation
person people[2];
people[0].name = "David";
people[0].number = "+1-949-468-2750";
```

```text
===================================================================================
             CUSTOM STRUCT MEMORY ENCAPSULATION
===================================================================================

  [ person: people[0] ]
  ├── name   ──> Pointer to "David"
  └── number ──> Pointer to "+1-949-468-2750"

===================================================================================
```

---

## 🧠 المحور الثاني: المقارنات الرياضية لخوارزميات الترتيب (`Sorting Benchmarks`)

### 1. جدول التعقيد الحسابي الشامل (`Asymptotic Benchmark Matrix`)

| اسم الخوارزمية (`Algorithm`) | أسوأ حالة (`Worst Case - Big O`) | أفضل حالة (`Best Case - Big Omega`) | الفكرة الأساسية (`Core Paradigm`) |
| :--- | :--- | :--- | :--- |
| **الترتيب الفقاعي (`Bubble Sort`)** | $O(n^2)$ | $\Omega(n)$ | مقارنة وتبديل أزواج متجاورة متتالية. |
| **ترتيب التحديد (`Selection Sort`)** | $O(n^2)$ | $\Omega(n^2)$ | البحث عن أصغر عنصر وإدراجه في البداية. |
| **الترتيب بالدمج (`Merge Sort`)** | $O(n \log n)$ | $\Omega(n \log n)$ | التقسيم إلى أنصاف وإعادة الدمج بترتيب. |
| **البحث الخطي (`Linear Search`)** | $O(n)$ | $\Omega(1)$ | البحث المتتالي عنصر بعنصر من البداية. |
| **البحث الثنائي (`Binary Search`)** | $O(\log n)$ | $\Omega(1)$ | تقسيم النطاق المنظم إلى نصفين متكررين. |

### 2. الفهم المعماري لـ $\Omega(1)$
في خوارزميات البحث، يمثل $\Omega(1)$ السيناريو الأكثر حظاً؛ وهو العثور على الهدف من أول محاولة (سواء في أول عنصر للبحث الخطي، أو في منتصف المصفوفة للبحث الثنائي).

---

## 🧬 المحور الثالث: ميكانيكا الذاكرة في التكرار الذاتي (`Recursive Stack Frames`)

### 1. إطارات المكدس (`Stack Frames`)
عندما تستدعي الدالة نفسها (مثل حساب المضروب `factorial` أو بناء هرم مارشال `draw`)، يقوم نظام التشغيل بحجز إطار جديد في مكدس الذاكرة (`Call Stack`) لكل استدعاء.
* **خطورة غياب الشرط الأساسي (`Base Case Hazard`)**: إذا لم تضع شرط توقف واضح، ستستمر الدوال في التراكم حتى تتجاوز المساحة المخصصة، مما يسبب الانهيار القاتل للنظام المعروف بـ `Stack Overflow`.

```text
===================================================================================
             RECURSIVE CALL STACK ACCUMULATION (PYRAMID)
===================================================================================

  [ Stack Top: draw(1) ] ──> Base Case Hit (Prints #, returns)
  [ Stack Mid: draw(2) ] ──> Waits for draw(1) to finish, prints ##
  [ Stack Bot: draw(3) ] ──> Waits for draw(2) to finish, prints ###
  [ main()             ] ──> Root execution frame

===================================================================================
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
