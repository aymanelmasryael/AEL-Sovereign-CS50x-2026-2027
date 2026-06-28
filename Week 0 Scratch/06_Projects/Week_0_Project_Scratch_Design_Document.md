# Engineering Projects: AEL System Design & Solution Blueprint
## Week 0: Advanced Scratch Mechanics, Oscartime & Ivy's Hardest Game Architecture

---

## 🏛️ المنهجية الهندسية لبناء مشاريع CS50x (AEL Engineering Paradigm)

> **"لن نكتب الحل مباشرة... سنمر بالمراحل المعمارية الصارمة لبناء البرمجيات."**

في بيئة عمل AEL، لا نتعامل مع مشروع `Scratch` كأداة رسومية بسيطة، بل نتعامل معه كنظام حقيقي متكامل يخضع لنفس معايير هندسة البرمجيات في كبرى الشركات العالمية (`Big Tech`). من خلال تحليل بيئة التطوير المتكاملة (`Scratch IDE`)، ندرك أن النظام يعتمد على نموذج الصندوق الأسود (`Black Box Model`) ومحاذاة دقيقة للذاكرة والشاشات.

---

## 📐 نظام الإحداثيات والفيزياء الرسومية (Coordinate System Physics)

```text
                  Y (0, 180)
                      │
                      │
                      │
X (-240, 0) ──────────┼────────── X (240, 0)  <-- (Center: 0, 0)
                      │
                      │
                      │
                  Y (0, -180)
```

### 💡 التشريح الرياضي لخشبة المسرح (The Stage)
تعمل بيئة `Scratch` على نظام إحداثيات ديكارتي (`Cartesian Coordinate System`) يحدد موقع الكائنات (`Sprites`) بدقة متناهية:
*   **المحور الأفقي (X-Axis):** يمتد من `-240` (أقصى اليسار) إلى `240` (أقصى اليمين). إجمالي العرض = `480 Pixels`.
*   **المحور الرأسي (Y-Axis):** يمتد من `-180` (أقصى الأسفل) إلى `180` (أقصى الأعلى). إجمالي الارتفاع = `360 Pixels`.
*   **نقطة الأصل (Origin):** المركز التام للمسرح يقع في الإحداثيات `(0, 0)`.

---

## 🚀 تشريح وهندسة الألعاب المتقدمة (Oscartime & Ivy's Hardest Game)

من خلال تفكيك شفرات المشاريع الكاملة التي استعرضها ديفيد مالان، نرى تطبيقاً عملياً لمفاهيم علوم الحاسب الأساسية مثل الدوال (`Functions`)، الشروط (`Conditionals`)، الحلقات (`Loops`)، والمتغيرات (`Variables`).

---

### 1. الهندسة المعمارية للعبة `Oscartime` (Event-Driven & Collision Physics)
لعبة `Oscartime` تعتمد على إسقاط النفايات من الأعلى ومحاولة التقاطها بواسطة شخصية "أوسكار".

```text
[ Timer Interrupt ] ──> [ Spawn Trash (X: Random, Y: 180) ] ──> [ Gravity Loop: Y = Y - 1 ] ──> [ Collision Check ]
```

#### 🔬 التفكيك البرمجي لكتل اللعبة (Block-by-Block Deconstruction):

##### أ. إدارة المظهر وتفاعل الفأرة (Costume Switch & Mouse Sensing)
```text
when green flag clicked
switch costume to oscar1
forever
    if touching mouse-pointer? then
        switch costume to oscar2
    else
        switch costume to oscar1
```
*   **التحليل الهندسي:** هذه الحلقة المستمرة (`forever`) تعمل كمستمع للأحداث (`Event Listener`). عندما تلامس إحداثيات الفأرة حدود كائن أوسكار، يقوم النظام بتبديل المؤشر الذاكري في الـ `VRAM` من مظهر `oscar1` (مغلق) إلى `oscar2` (مفتوح).

##### ب. فيزياء السقوط الحر والتوليد العشوائي (Gravity & PRNG)
```text
when green flag clicked
set drag mode draggable
go to x: pick random 0 to 240 y: 180
forever
    change y by -1
```
*   **وضع السحب (`set drag mode draggable`):** يتيح تفعيل واجهة برمجة تطبيقات السحب والإفلات (`Drag-and-Drop API`) لتمكين المستخدم من التقاط النفايات أثناء سقوطها.
*   **التوليد العشوائي (`pick random 0 to 240`):** يستدعي مولد الأرقام شبه العشوائية (`PRNG`) لتعيين نقطة البداية على المحور `X`، بينما يظل الارتفاع ثابتاً في القمة `Y: 180`.
*   **حلقة الجاذبية (`change y by -1`):** تخفض الإحداثي الرأسي بمقدار بيكسل واحد في كل إطار، محاكية السقوط الحر.

##### ج. التجريد الوظيفي لإعادة الإرسال (Custom Block: `go to top`)
```text
define go to top
    go to x: pick random 0 to 240 y: 180

when green flag clicked
forever
    if touching Oscar? then
        go to top
```
*   **الذروة الهندسية:** بدلاً من تكرار كود إعادة تهيئة الإحداثيات، تم إنشاء وظيفة مخصصة (`define go to top`). عند حدوث التصادم (`touching Oscar?`)، يُستدعى هذا العنوان الذاكري ليعيد النفاية فوراً إلى القمة.

---

### 2. الهندسة المعمارية للعبة `Ivy's Hardest Game` (Complex State Machines & Vector Motion)
تمثل هذه اللعبة قمة التحدي البرمجي في الأسبوع الافتتاحي، حيث تضم 3 كائنات مستقلة تتفاعل وتتصادم في الوقت الفعلي (`Real-time Concurrency`).

```text
+-----------------------------------------------------------------------------------+
|                        Master Concurrency Loop (3 Sprites)                        |
|                                                                                   |
|  [ Thread 1: Harvard Sprite ] <──> [ Thread 2: Yale Obstacle ] <──> [ Thread 3: MIT Chaser ] |
+-----------------------------------------------------------------------------------+
```

#### 🔬 التفكيك البرمجي لكتل اللعبة (Block-by-Block Deconstruction):

##### أ. كائن اللاعب (Harvard Sprite - Keyboard Listening & Wall Sensing)
```text
when green flag clicked
go to x: 0 y: 0
forever
    listen for keyboard
    feel for walls

define listen for keyboard
    if key up arrow pressed? then change y by 1
    if key down arrow pressed? then change y by -1
    if key right arrow pressed? then change x by 1
    if key left arrow pressed? then change x by -1

define feel for walls
    if touching left wall? then change x by 1
    if touching right wall? then change x by -1
```
*   **الملاحة وإدارة الحلقة الرئيسية:** عند البدء، يتجه اللاعب للمركز `(0, 0)`. الحلقة المستمرة تستدعي دالتين منفصلتين: `listen for keyboard` و `feel for walls`.
*   **تجريد مدخلات لوحة المفاتيح (`listen for keyboard`):** يتم فحص مسجلات المقاطعة (`Interrupt Registers`) لأسهم اللوحة الأربعة، وتُعدل إحداثيات `X` و `Y` صعوداً أو هبوطاً.
*   **مستشعرات الجدران (`feel for walls`):** وظيفة مخصصة لحماية اللاعب من الخروج عن المسرح (`Boundary Check`). إذا تداخل اللاعب مع الجدار الأيسر، يُدفع فوراً خطوة لليمين (`change x by 1`) ليبقى في منطقة آمنة.

##### ب. كائن العائق الأوتوماتيكي (Yale Sprite - Autonomous Patrol Loop)
```text
when green flag clicked
go to x: 0 y: 0
point in direction 90
forever
    if touching left wall? or touching right wall? then
        turn 180 degrees
    move 1 steps
```
*   **التحليل الهندسي:** يعمل كائن `Yale` كحارس عائق يدور في مسار ثابت. يستخدم الشرط المركب (`Compound Boolean Expression`): `if touching left OR right`. عند الاصطدام، يتم تعديل زاوية متجه الحركة بمقدار `180 درجة` (عكس الاتجاه)، مع الاستمرار في التقدم بسرعة `move 1 steps`.

##### ج. كائن التتبع الذكي (MIT Sprite - AI Pathfinding / Tracking Simulation)
```text
when green flag clicked
go to random position
forever
    point towards Harvard
    move 1 steps
```
*   **محاكاة خوارزميات التتبع (`AI Tracking Simulation`):** يبدأ كائن `MIT` في نقطة عشوائية، وفي كل دورة معالج يقوم بحساب حسابات المثلثات (`Trigonometry - atan2`) لمعرفة زاوية كائن `Harvard` عبر كتلة `point towards`، ثم يتقدم نحوه بخطوة واحدة، مما يخلق محاكاة ذكية للمطاردة!

---

## 📐 مقارنة الحلول والمعايير القياسية (Big Tech Architectural Standards)

| المعيار الهندسي | تطبيق معايير Google (Modularity) | تطبيق معايير Apple (Aesthetics) | تطبيق معايير Linux Kernel (Performance) |
| :--- | :--- | :--- | :--- |
| **إدارة الكائنات (Sprites)** | فصل كل كائن في وحدة مستقلة (`Component Driven`) مع تجنب التداخل المباشر. | استخدام رسوميات متجهة (`SVG`) فائقة الدقة لتجنب البكسلة عند تغيير الحجم. | التخلص من الكائنات غير النشطة فوراً (`Garbage Collection / Delete Clone`) لتوفير الذاكرة. |
| **التواصل بين الكتل (Events)** | استخدام نظام النشر والاشتراك (`Pub/Sub`) عبر كتل `broadcast` و `when I receive`. | مزامنة الأحداث الرسومية مع المؤثرات الصوتية لخلق تجربة سلسة (`Micro-animations`). | تقليل عدد رسائل الـ `broadcast` التنافسية لمنع حدوث اختناق في خط الأنابيب (`Event Congestion`). |
| **حلقات التكرار (Game Loops)** | تجميع الشروط المنطقية داخل وظيفة مخصصة واحدة (`Custom Block / Clean Code`). | ضبط التوقيتات لمنع تقطيع الصور (`Frame Drops`) أثناء الانزلاق `Glide`. | تجنب استخدام `forever` بدون شروط قاطعة، واستبدالها بـ `repeat until` لتقليل استهلاك الـ CPU. |
