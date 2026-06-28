# AEL CS Encyclopedia™ – Week 4 Memory: Practical Reverse Engineering (AR)

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
  "domain": "Week 4 Memory: Bare-Metal File Header Decoding & Magic Numbers",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🚀 الهندسة العكسية التطبيقية: فك شفرة ترويسات الملفات والأرقام السحرية (`File Forensics`)

في الهندسة العكسية التطبيقية، نتخلى عن ملحقات الملفات الوهمية (`.pdf, .jpg, .wav`) ونعامل الملف كـ **تدفق من البايتات الخام (`Raw Byte Stream`)** للبحث عن الأرقام السحرية الحاكمة (`Magic Numbers`).

---

## 🛡️ ركائز الهندسة العكسية للذاكرة (`Reverse Engineering Pillars`)

### 1. تشريح الأرقام السحرية لملفات `JPEG` (`Magic Number Verification`)
* **البصمة الحيوية للملف**: أي ملف `JPEG` حقيقي يجب أن تبدأ البايتات الأربعة الأولى منه بالترتيب الست عشري الصارم التالي: `0xFF`, `0xD8`, `0xFF`, والبايت الرابع يكون من `0xE0` إلى `0xEF`.
* **البروتوكول الهندسي**: لا يمكن الوثوق بامتداد الملف الخارجي. يقوم المحقق الجنائي بفتح الملف باستخدام `fopen(file, "rb")`، وقراءة البايتات الأربعة الأولى لمطابقتها مع البصمة الشرعية.

```text
===================================================================================
             JPEG MAGIC NUMBER FORENSIC SIGNATURE MATRIX
===================================================================================

  [ BYTE 0 ]   :   0xFF  (1111 1111)
  [ BYTE 1 ]   :   0xD8  (1101 1000)
  [ BYTE 2 ]   :   0xFF  (1111 1111)
  [ BYTE 3 ]   :   0xE0 .. 0xEF  (Matches bitwise: (buffer[3] & 0xF0) == 0xE0)

===================================================================================
```

---

### 2. تشريح ترويسة ملفات الصور `BITMAP` (`BMP Header Decoding`)
* **الهيكل العكسي**: تتكون ترويسة ملفات `BMP` من كتلتين رئيسيتين:
  * `BITMAPFILEHEADER` (14 بايت): تحتوي على توقيع الملف `BM` (`0x42 0x4D`) وإجمالي حجم الملف.
  * `BITMAPINFOHEADER` (40 بايت): تحتوي على أبعاد الصورة (`Width & Height`) وعمق الألوان (`BitCount`).
* **الهندسة العكسية للبكسل**: تُخزن بكسلات الـ `BMP` في الذاكرة بنظام `BGR` (أزرق، أخضر، أحمر) وليس `RGB`، ويجب الحذر من بايتات الحشو (`Padding`) في نهاية كل صف.

---

### 3. تقنيات استعادة البيانات المفقودة (`File Carving`)
* **فكرة عمل الاستعادة**: عند حذف ملف من نظام التشغيل، فإن النظام يحذف فقط الفهرس (`Pointer`) ولكنه يترك كتل البايتات الفعلية في قطاعات القرص (`Disk Blocks`).
* **تطبيق `File Carving`**: يقوم برنامج الفحص بالبحث عن توقيعات البداية (`Magic Numbers`) في الذاكرة الخام، ويبدأ في نسخ البايتات إلى ملف جديد حتى يعثر على توقيع ملف آخر.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
