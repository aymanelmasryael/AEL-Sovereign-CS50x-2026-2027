# AEL CS Encyclopedia™ – Week 8: Deep Internal Analysis (AR)

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
  "domain": "Week 8 (HTML, CSS, JavaScript): DOM Architecture, Event Loops & UI Glassmorphism",
  "document_type": "04_Deep_Internal_Analysis (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 التحليل الداخلي العميق: دورة حياة الحدث وأشجار تصيير المتصفح (Render Trees)

### 1. دورة حياة التقاط الأحداث وتصعيدها (`Event Capturing & Bubbling`)
عندما ينقر المستخدم على زر داخل صفحة الويب، لا يتم إشعار الزر بشكل منعزل، بل يمر الحدث بسلسلة هرمية من مرحلتين:
*   **مرحلة الالتقاط (`Event Capturing`)**: ينحدر الحدث من أعلى قمة شجرة الـ `DOM` (`window` ثم `document` ثم `body`) وصولاً إلى العنصر المستهدف.
*   **مرحلة التصعيد (`Event Bubbling`)**: بمجرد تفعيل الدالة المرتبطة بالعنصر المستهدف (`Target Element`)، يتصاعد الحدث فقاعياً عائداً إلى الأصول العليا، مما يسمح بتطبيق مبدأ تفويض الأحداث (`Event Delegation`).

```text
===================================================================================
                   EVENT CAPTURING & BUBBLING MATRIX
===================================================================================

        [ window / document ]   ──( 1. Capturing Down )──>   │
                 │                                           │
             [ body / div ]                                  │  ( 2. Bubbling Up )
                 │                                           │         ▲
           [ button #ael-btn ]  <──( Target Activation )─────┘         │

===================================================================================
```

### 2. شجرة التصيير وإعادة التدفق (`Render Tree & Reflow Calculation`)
*   **دمج الهياكل (`The Combine Matrix`)**: يقوم محرك المتصفح بدمج شجرة الـ `DOM` (المنبثقة من HTML) مع شجرة الـ `CSSOM` (المنبثقة من CSS) لإنتاج شجرة التصيير النهائية (`Render Tree`).
*   **إعادة الحساب (`Reflow / Repaint`)**: تعديل خصائص التخطيط (`Width / Height / Margin`) عبر الـ JavaScript يجبر المتصفح على إعادة حساب أبعاد كافة العناصر الهرمية (`Reflow`)، وهي عملية مكلفة حسابياً. لذا، تركز موسوعتنا على التلاعب بخصائص الـ `Transform` و `Opacity` المدعومة مباشرة عبر مسرعات الرسوميات (`GPU Acceleration`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
