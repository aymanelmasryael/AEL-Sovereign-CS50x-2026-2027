# Practical Reverse Engineering: Week 0 - Scratch (AST Forensics & .sb3 Deconstruction)

---

## 🔬 الهندسة العكسية لملفات `src0/*.sb3` (Bare-Metal AST Forensics)

في هندسة النظم العميقة، لا ننظر إلى ملفات `Scratch (.sb3)` الموجودة في مجلد `src0` كأيقونات بصرية مغلقة، بل كحاويات برمجية قياسية (`Containerized Archives`).

```text
[ Sample.sb3 ] ──(Unzip Archive)──> [ project.json (Abstract Syntax Tree) ] + [ 83a9b2...svg (Vector Assets) ] + [ 0d3b...wav (PCM Audio) ]
```

### 💡 حقيقة ملف الـ `.sb3`:
ملف `sb3` هو في الواقع ملف مضغوط بصيغة `ZIP Archive`. إذا قمت بتغيير امتداد أي ملف من `hello0.sb3` إلى `hello0.zip` وفك ضغطه، ستجد بداخله ملفاً جوهرياً يسمى `project.json`. هذا الملف يحتوي على شجرة البنية المجرّدة (`Abstract Syntax Tree - AST`) التي تُعرّف كل كتلة كائن كعقدة برمجة صريحة (`JSON Object`) يقرأها محرك الجافاسكريبت.

---

## 📦 1. تشريح سلسلة `hello` (من النصوص الثابتة إلى المعاملات الديناميكية)

يحتوي مجلد `src0` على التطور المعماري لسلسلة `hello` عبر 3 ملفات مرجعية:

### 📂 روابط الملفات المصدرية المباشرة في `src0`:
*   [hello0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello0.sb3): الكتلة الأساسية الثابتة (`say [hello, world]`).
*   [hello1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello1.sb3): إدخال المستشعر الحواري (`ask [What's your name?] and wait`).
*   [hello2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/hello2.sb3): دمج المتغيرات عبر دالة الـ `join` (`say (join (hello, ) (answer))`).

#### 🔬 ماذا يحدث داخل `project.json` عند الترقية إلى `hello2.sb3`؟
عندما نفتح الـ `project.json` لملف `hello2.sb3`، نجد أن كائن الـ `say` يتوقف عن استقبال نص خام (`String Literal`). بدلاً من ذلك، يُمرر له كائن فرعي (`Nested Opcode`) يحمل اسم `operator_join`، والذي يستقبل مدخلين: النص الثابت `"hello, "` ومتغير مستشعر الذاكرة `sensing_answer`.

---

## 📦 2. تشريح سلسلة `meow` (الهندسة العكسية لسُلّم التجريد)

تستعرض ملفات `meow0` إلى `meow4` درجات سُلّم التجريد الأربعة داخل بنية الـ `AST`.

### 📂 روابط الملفات المصدرية المباشرة في `src0`:
*   [meow0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow0.sb3): الأكواد المكررة يدوياً (3 كتل متتالية).
*   [meow1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow1.sb3): حلقة التحكم التكرارية `repeat (3)`.
*   [meow2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow2.sb3) & [meow3.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow3.sb3): بناء الوظيفة المخصصة (`define meow`).
*   [meow4.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/meow4.sb3): الذروة الهندسية؛ الوظيفة المعاملية (`define meow (n)`).

#### 🔬 الهندسة العكسية للكتلة المخصصة `define meow (n)`:
في ملف `meow4.sb3`، يقوم محرك `Scratch` بإنشاء عقدة برمجية خاصة في `project.json` تسمى `procedures_definition`. تحتوي هذه العقدة على جدول رموز محلي (`Symbol Table / Mutation Block`) يحفظ المتغير `n`. عند استدعاء الوظيفة في الكود الرئيسي عبر `procedures_call`، يقوم النظام بتنفيذ قفز ذاكري (`Function Call Stack`) ممرراً الرقم المطلوب ديناميكياً.

---

## 📦 3. تشريح سلسلة `pet` (الاستعلام المستمر مقابل المقاطعات العتادية)

تظهر المقارنة المعمارية الأوضح لكفاءة المعالج في فحص ملفي `pet0` و `pet1`.

### 📂 روابط الملفات المصدرية المباشرة في `src0`:
*   [pet0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/pet0.sb3): حلقة الاستعلام العمياء (`forever -> if touching mouse-pointer?`).
*   [pet1.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/pet1.sb3): المقاطعة العتادية الذكية (`when video motion > 10`).

#### 🔬 التقييم المعماري في الـ AST:
*   في `pet0.sb3`، كائن الـ `control_forever` يُلزم المعالج بدفع دورات تنفيذ متتالية (`CPU Polling Loop`).
*   في `pet1.sb3`، يختفي الـ `control_forever` تماماً! وتظهر عقدة رئيسية `videoSensing_whenMotionGreaterThan` تعمل كمستمع أحداث مستقل (`Event Listener / Hook`) يتفاعل فقط عند ورود تنبيه عتادي من حساس الكاميرا.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
