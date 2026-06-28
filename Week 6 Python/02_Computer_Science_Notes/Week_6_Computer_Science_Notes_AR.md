# AEL CS Encyclopedia™ – Week 6 Python: Computer Science Notes (AR)

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
  "domain": "Week 6 Python: Theoretical Concepts & Language Pragmatics",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🧠 مذكرات علوم الحاسب: البنية التجريدية والمقارنة مع C

تمثل لغة `Python` تحولاً فيกระบวนทัศน์ البرمجة (`Programming Paradigm`). بدلاً من التركيز على تفاصيل العتاد الصلب (`Hardware Specifics`)، تركز `Python` على سرعة التطوير ونظافة الشفرة البرمجية.

### 1. تصنيف اللغات: المجمعة مقابل المفسرة (`Compiled vs. Interpreted`)
*   **اللغات المجمعة (`C`)**: يتم تحويل الشفرة المصدرية بالكامل عبر المترجم (`Compiler`) إلى لغة الآلة (`Machine Code`) المخصصة للمعالج ونظام التشغيل قبل التشغيل.
*   **اللغات المفسرة (`Python`)**: يقوم المحرك الافتراضي `CPython` بتحويل الشفرة المصدرية إلى رمز وسيط (`Bytecode`)، ثم يقوم بتنفيذه سطراً بسطر في وقت التشغيل (`Runtime`). هذا يوفر قابلية تشغيل فائقة عبر المنصات المختلفة (`Cross-Platform Portability`).

### 2. الكتابة الديناميكية مقابل الثابتة (`Dynamic vs. Static Typing`)
*   **الكتابة الثابتة (`Static Typing`)**: في `C`، يجب تعريف نوع المتغير صراحةً (`int, char, float`) ولا يمكن تغييره لاحقاً.
*   **الكتابة الديناميكية (`Dynamic Typing`)**: في `Python`، ترتبط الأنواع بالقيمة المخزنة وليس باسم المتغير نفسه، مما يتيح تغيير نوع البيانات المخزنة بمرونة أثناء تنفيذ البرنامج.

```text
===================================================================================
                   TYPING PARADIGM COMPARISON
===================================================================================

  [ C: int x = 10; ]     ──> Fixed 4 bytes on stack, strict type validation
  [ Python: x = 10 ]     ──> Dynamic Object Reference, mutable at runtime

===================================================================================
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
