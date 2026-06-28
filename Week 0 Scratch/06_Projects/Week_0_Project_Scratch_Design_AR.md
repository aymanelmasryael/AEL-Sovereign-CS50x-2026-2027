# Master Projects Architecture: Week 0 - Scratch (Oscartime & Ivy's Hardest Game)

---

## 🚀 مشاريع التخرج المصغرة في مجلد `src0` (Cartesian Physics & System Execution)

يستعرض ديفيد مالان في المجلد المصدر `src0` التطور المعماري الكامل لبناء لعبتين متكاملتين خطوة بخطوة. في منهجية AEL، نعتبر هذه الألعاب تطبيقاً حرفياً لمعمارية المستشعرات، ومحاكاة الفيزياء، والمستوى الديكارتي (`Cartesian Coordinates Plane`).

```text
[ Canvas Plane ] ──> [ X: -240 to 240 / Y: -180 to 180 ] ──> [ Collision Engine (Sensing) ]
```

---

## 🎮 1. الهندسة المعمارية لمشروع `Oscartime` (فيزياء السقوط الحر والتسجيل)

يتدرج بناء `Oscartime` عبر 6 مراحل متتالية موجودة في `src0`، وصولاً إلى النسخة النهائية `Oscartime.sb3`.

### 📂 روابط ملفات `Oscartime` المباشرة في `src0`:
*   [Oscartime - 0 (Malan).sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime%20-%200%20(Malan).sb3): تهيئة الكائن وسقوطه الرأسي الأولي.
*   [Oscartime - 3 (Malan).sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime%20-%203%20(Malan).sb3): إضافة كاشف الاصطدام بسلة المهملات (`touching trash can?`).
*   [Oscartime.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Oscartime.sb3): النسخة المجمعة بالكامل مع نظام احتساب النقاط (`Score Variable`).

### 🔬 التفكيك الفيزيائي لمحرك اللعبة:
*   **محاكاة الجاذبية (Gravity Simulation):**
    لا يمتلك `Scratch` محرك جاذبية مدمج. بدلاً من ذلك، نستخدم حلقة `forever` تحتوي على الكتلة `change y by -5`. هذا يوجه معالج الرسومات بخفض إحداثي الصادات بمقدار 5 بيكسلات في كل دورة معالجة (`Frame Update`)، مما يخلق محاكاة بصرية سلسة للسقوط الحر.
*   **العشوائية الديكارتية (Cartesian Randomness):**
    لجعل اللعبة غير متوقعة، يتم إعادة توجيه النفايات عند سقوطها إلى القمة (`y: 180`) مع اختيار موقع أفقي عشوائي عبر المعامل `pick random -240 to 240`.
*   **نظام التقييم (Mutator Variable):**
    متغير الـ `score` هو عنوان ذاكري صريح. عند تحرير شرط الاصطدام (`if touching trash can?`)، يقوم المعالج بإرسال تعليمات تعديل ذاكرية `change score by 1`، مصحوباً بأثر جانبي صوتي (`Play sound pop`).

---

## 🎮 2. الهندسة المعمارية لمشروع `Ivy's Hardest Game` (محرك الاصطدام والجدران)

تمثل لعبة `Ivy's Hardest Game` قمة التعقيد الهندسي في الأسبوع 0، حيث تتناول الاستشعار التفاعلي للوحة المفاتيح والفيزياء الارتدادية للجدران.

### 📂 روابط ملفات `Ivy's Hardest Game` المباشرة في `src0`:
*   [Ivy's Hardest Game - Harvard 0.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard%200.sb3): تهيئة حركة اللاعب عبر كتل المفاتيح.
*   [Ivy's Hardest Game - Harvard 2.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard%202.sb3): بناء الكتل المخصصة (`listen for keyboard` و `feel for walls`).
*   [Ivy's Hardest Game - Harvard.sb3](file:///Volumes/Apple/انظمه%20تم%20تطويرها%20/نظام%20جديد%20/cs50x%202026/cs50x%202026-2027/src0/Ivy's%20Hardest%20Game%20-%20Harvard.sb3): المشروع العملاق النهائي مع العوائق المتحركة ونظام الخسارة.

### 🔬 تشريح هندسة الوظائف المخصصة (Modular Sub-Routines):
بدلاً من حشو حلقة الـ `forever` بـ 20 شرطاً متداخلاً، يقدم مالان تصميماً معمارياً شديد النظافة قائماً على فصل الاهتمامات (`Separation of Concerns`).

```text
when [Green Flag] clicked
 ├── go to x: 0 y: 0 (Reset Vector)
 └── forever
      ├── listen for keyboard (Active Input Polling)
      └── feel for walls      (Bounding Box Collision Engine)
```

#### 1. روتين `listen for keyboard`:
يتحقق من الحالات الأربع للأسهم (`up, down, left, right`) ويغير قنوات الإحداثيات (`change x by 1` / `change y by 1`).
#### 2. روتين `feel for walls` (محرك الصد العكسي):
عند ملامسة الجدار الأيمن (`if touching right wall?`)، يقوم النظام على الفور بمعاكسة المتجه الحركي عبر `change x by -1`. هذا الإلغاء اللحظي للإزاحة يمنع اختراق الكائن للجدار (`Clipping Prevention / Physics Bounding Box`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
