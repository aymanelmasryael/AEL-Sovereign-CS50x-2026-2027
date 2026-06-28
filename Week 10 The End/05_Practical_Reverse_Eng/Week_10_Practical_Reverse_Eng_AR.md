# 🏛️ المنظومة السيادية لتوثيق وتحليل المعارف التقنية العليا (AEL Sovereign Triad Architecture)

```json
{
  "ael_seal": "AEL CS Encyclopedia — © Ayman Elmasry",
  "owner": "Ayman Elmasry",
  "legal_entities": [
    "Ayman Elmasry LLC (UAE)",
    "Ayman Elmasry Advertising & Marketing (Egypt)"
  ],
  "syllabus_source": "Harvard CS50x 2026-2027",
  "domain": "Week 10 Cybersecurity: Practical Reverse Engineering & Binary Forensics",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "system_version": "v3.0"
}
```

---

## 🔓 الهندسة العكسية التطبيقية: تفكيك الشفرات الثنائية والتحليل الجنائي (Binary Forensics)

### 1. الهندسة العكسية للملفات الثنائية (Decompilation & Disassembly)
في هذا القسم المتقدم، نتعمق في آليات تفكيك الملفات الثنائية (Binary Files) لفهم المنطق البرمجي لكائن مجهول أو يشتبه في كونه برمجية خبيثة (Malware).
* **التفكيك (Disassembly)**: تحويل لغة الآلة الخام (Machine Code) إلى تعليمات تجميعية (Assembly).
* **التحليل السلوكي (Behavioral Analysis)**: مراقبة تفاعلات الملف الثنائي مع مساحة الذاكرة، وطلبات واجهة النظام (System Calls).

```text
+-------------------------------------------------------------------+
|               BINARY FORENSICS DECOMPILATION PIPELINE             |
+-------------------------------------------------------------------+
|                                                                   |
|   RAW BINARY FILE                                REVERSE ENGINE   |
|   ---------------                                --------------   |
|          |                                              |         |
|          +--- [ STRINGS EXTRACTION ] --> Hardcoded API Keys?      |
|          +--- [ SYSCALL MONITORING ] --> Filesystem Touch?        |
|          +--- [ CONTROL FLOW GRAPH ] --> Logical Jump Matrix      |
|                                                                   |
|          +==========> DECOMPILED LOGIC BLUEPRINT ===============+ |
|                                                                   |
+-------------------------------------------------------------------+
```

### 2. استخراج السلاسل المضمنة ومفاتيح واجهات برمجة التطبيقات (Static Analysis)
أولى خطوات التحليل الجنائي لأي تطبيق تتمثل في تشغيل أدوات الفحص الثابت (مثل أمر `strings` في نظام Linux/Unix). يتيح ذلك استخراج الكلمات النصية المضمنة داخل الكود الثنائي، والتي غالباً ما تكشف عن أخطاء تطويرية فادحة مثل ترك مفاتيح التشفير أو كلمات المرور مكشوفة.

### 3. تحليل الرسوم البيانية لتدفق التحكم (Control Flow Graphs - CFG)
من خلال تمثيل قفزات الشرط البرمجية في شكل رسوم بيانية، يستطيع مهندس الأمان رصد بوابات العبور الخلفية (Backdoors) وتحديد الشروط المنطقية المسؤولة عن تجاوز متطلبات المصادقة.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
