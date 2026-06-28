# AEL CS Encyclopedia™ – Week 3 Algorithms: Deep Internal Analysis (AR)

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
  "domain": "Week 3: Mathematical Recurrences & Memory Padding",
  "document_type": "04_Deep_Internal_Analysis (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: الإثبات الرياضي لخوارزمية الترتيب بالدمج (`Merge Sort Proof`)

### 1. شجرة التكرار الذاتي (`Recursion Tree Method`)
لفهم كيف تقفز خوارزمية `Merge Sort` بالكفاءة من $O(n^2)$ إلى $O(n \log n)$، نقوم بتفكيك معادلة التكرار الرياضية (`Recurrence Relation`):
$$T(n) = 2T\left(\frac{n}{2}\right) + O(n)$$
* **التشريح الرياضي**:
  * تقسيم المصفوفة إلى نصفين يستغرق وقتاً ثابتاً $O(1)$.
  * استدعاء الدالة لكل نصف ينتج $2T(n/2)$.
  * دمج النصفين يتطلب فحص كل عنصر، مما يستهلك $O(n)$.

```text
===================================================================================
             MERGE SORT RECURSION TREE DECONSTRUCTION
===================================================================================

  Level 0:                     [ n ]                     ──> Work: O(n)
                             /       \
  Level 1:             [ n/2 ]       [ n/2 ]             ──> Work: 2 * (n/2) = O(n)
                      /      \       /      \
  Level 2:        [ n/4 ] [ n/4 ] [ n/4 ] [ n/4 ]        ──> Work: 4 * (n/4) = O(n)
  
  Total Depth = log_2(n)  ──> Total Execution Cost: O(n * log n)

===================================================================================
```

---

## 🧠 المحور الثاني: محاذاة الذاكرة وحشو الهياكل (`Struct Padding & Alignment`)

### 1. خفايا الذاكرة في الهياكل (`Memory Alignment Boundaries`)
عند تعريف هيكل `struct` في لغة `C`، قد تظن أن حجم الهيكل هو بالضبط مجموع أحجام عناصره. ولكن المعالج يفرض قيوداً هندسية تُعرف بمحاذاة الذاكرة (`Memory Alignment`).
* **ظاهرة الحشو (`Padding`)**: يفضل المعالج قراءة البيانات في كتل من 4 أو 8 بايت. إذا كان الهيكل يحتوي على `char` (1 بايت) متبوعاً بـ `int` (4 بايت)، يقوم المترجم بإقحام 3 بايتات فارغة (`Padding Bytes`) لضمان محاذاة المتغير التالي.

```text
===================================================================================
             PHYSICAL STRUCT MEMORY PADDING (64-BIT ARCH)
===================================================================================

  struct { char c; int i; };
  
  [ 0x100: char c (1B) ] [ 0x101-0x103: PADDING (3B) ] [ 0x104-0x107: int i (4B) ]
  
  Total Size in RAM = 8 Bytes (not 5 Bytes!)

===================================================================================
```

---

## 🧬 المحور الثالث: تشريح المكدس أثناء التكرار العميق (`Stack Frame Dynamics`)

### 1. الهيكل الداخلي لإطار المكدس (`Anatomy of an Activation Record`)
كلما استدعيت دالة תكرارية، يحجز المترجم إطاراً يحتوي على:
1. **عنوان العودة (`Return Address`)**: أين يعود البرنامج بعد انتهاء الدالة.
2. **المعاملات (`Arguments`)**: القيم الممررة للدالة.
3. **المتغيرات المحلية (`Local Variables`)**: المتغيرات المؤقتة.

إذا تجاوز عمق الاستدعاءات التكرارية سعة الـ `Stack` (والتي تكون عادةً بضعة ميجابايتات في أنظمة التشغيل الحديثة)، يتدخل معالج حماية الذاكرة (`MMU`) ويرسل إشارة القتل `SIGSEGV` ليحبط البرنامج فوراً.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
