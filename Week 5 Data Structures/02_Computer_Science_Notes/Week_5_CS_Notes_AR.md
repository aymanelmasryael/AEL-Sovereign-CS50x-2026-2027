# AEL CS Encyclopedia™ – Week 5 Data Structures: CS Notes (AR)

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
  "domain": "Week 5 Data Structures: Abstract Data Types & Asymptotic Topology",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: الهياكل التجريدية (`Abstract Data Types - ADTs`)

### 1. مفهوم التجريد في بنية البيانات
تمثل الهياكل التجريدية (`ADTs`) مثل الطوابير (`Queues`) والمكدسات (`Stacks`) واجهات قياسية للتعامل مع البيانات دون التقيد بتفاصيل تطبيقها في الذاكرة. يمكن تطبيق المكدس (قاعدة `LIFO: Last In, First Out`) أو الطابور (قاعدة `FIFO: First In, First Out`) إما عبر مصفوفة ثابتة أو قائمة مرتبطة ديناميكية.

```text
===================================================================================
             STACK (LIFO) VS QUEUE (FIFO) EXECUTION
===================================================================================

  STACK (LIFO)                            QUEUE (FIFO)
  ┌──────────┐                            ┌─────┬─────┬─────┬─────┐
  │  Top     │ ◄── Push / Pop             │  1  │  2  │  3  │  4  │ ──► Dequeue
  ├──────────┤                            └─────┴─────┴─────┴─────┘
  │  Bottom  │                               ▲
  └──────────┘                               └─ Enqueue

===================================================================================
```

---

## 🧠 المحور الثاني: التعقيد الخوارزمي والمقارنة المعيارية (`Asymptotic Topology`)

### 1. جدول المقارنة الشاملة (`Master Asymptotic Trade-off Matrix`)

| بنية البيانات (`Data Structure`) | تكلفة البحث (`Search`) | تكلفة الإدراج (`Insert`) | تكلفة الحذف (`Delete`) | الملاحظات الفنية (`Engineering Note`) |
| :--- | :--- | :--- | :--- | :--- |
| **المصفوفة الثابتة (`Static Array`)** | `O(1)` (بفهرس) / `O(n)` | `O(n)` (مع إزاحة) | `O(n)` (مع إزاحة) | تمتاز بتجاور الذاكرة وكفاءة التخزين المؤقت (`Cache Locality`). |
| **القائمة المرتبطة (`Singly Linked List`)** | `O(n)` (بحث خطي) | `O(1)` (عند الرأس) | `O(1)` (مع معرفة المؤشر) | لا توجد حاجة لإعادة التخصيص `realloc`، ولكن تفقد خاصية البحث الثنائي. |
| **جدول التجزئة (`Hash Table - Chaining`)** | `O(1)` (المتوسط) / `O(n)` | `O(1)` (عند الرأس) | `O(1)` (عند الرأس) | الأداء يعتمد كلياً على كفاءة دالة التجزئة (`Hash Function Factor`). |
| **شجرة التريس (`Trie Tree`)** | `O(k)` (طول الكلمة) | `O(k)` (طول الكلمة) | `O(k)` (طول الكلمة) | استهلاك هائل للذاكرة (`Memory Overhead`) مقابل سرعة فائقة وثابتة. |
| **شجرة ثنائية متزنة (`Balanced BST`)** | `O(log n)` | `O(log n)` | `O(log n)` | تضمن أداءً لوغاريتمياً مستقراً ولكن تتطلب خوارزميات توازن معقدة. |

---

## 🧬 المحور الثالث: دوال التجزئة ومعالجة التصادم (`Hash Collisions`)

### 1. حتمية التصادم الرياضي
وفقاً لمبدأ برج الحمام (`Pigeonhole Principle`)، إذا كان عدد المفاتيح المحتملة أكبر من حجم جدول التجزئة `N`، فإن حدوث التصادم (`Collision`) حتمي رياضياً.

### 2. آليات الحل المعماري
*   **العنونة المفتوحة (`Open Addressing - Linear Probing`)**: عند التصادم، يبحث النظام عن الفتحة التالية الفارغة في المصفوفة. يعاني هذا النهج من مشكلة التكتل (`Clustering`).
*   **السلسلة المنفصلة (`Chaining`)**: يتم تحويل كل فتحة في الجدول إلى رأس قائمة مرتبطة (`Header Pointer`). هذا هو النهج الأكثر استقراراً في الأنظمة التي يتغير حجم بياناتها باستمرار.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
