# AEL CS Encyclopedia™ – Week 6 Python: Practical Reverse Engineering (AR)

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
  "domain": "Week 6 Python: Bytecode Disassembly & Runtime Inspection",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🛠️ الهندسة العكسية التطبيقية: تفكيك الـ Bytecode وفحص CPython

لفهم الفارق الحقيقي في التكلفة الحسابية بين `C` و `Python`، نقوم بتطبيق تقنيات الهندسة العكسية وفحص الشفرة الوسيطة (`Bytecode Inspection`) باستخدام وحدة `dis`.

### 1. تفكيك الشفرة باستخدام مكتبة dis (`Bytecode Disassembly`)
عند فحص حلقة تكرارية بسيطة في `Python`، تظهر لنا الأوامر الافتراضية التي ينفذها محرك `CPython`:
*   `LOAD_FAST / STORE_FAST`: قراءة وكتابة المتغيرات المحلية من الذاكرة التخزينية الافتراضية.
*   `FOR_ITER`: فحص المعاير الداخلي للكائن واستدعاء `__next__`، مما يستهلك دورات معالج إضافية مقارنة بقفزات الـ `Assembly` في لغة `C`.

### 2. تدقيق الأداء ومراقبة استهلاك الذاكرة (`Memory Overhead Auditing`)
*   **تتبع الحجم الفعلي للكائنات (`sys.getsizeof`)**: في `C`، يستهلك المتغير من نوع `int` 4 بايت فقط. أما في `Python`، يستهلك الرقم الصحيح 28 بايت أو أكثر بسبب بنية الـ `PyObject` التي تحتفظ بعدداد المراجع ونوع الكائن.

```text
===================================================================================
                   BYTECODE DISASSEMBLY MATRIX
===================================================================================

  1           0 LOAD_CONST               0 (0)
              2 STORE_FAST               0 (x)
  2           4 LOAD_FAST                0 (x)
              6 LOAD_CONST               1 (1)
              8 BINARY_ADD
             10 STORE_FAST               0 (x)

===================================================================================
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
