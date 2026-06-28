# AEL Sovereign C Masterpieces: Custom `ael_src1` Base

---

## 🏛️ فلسفة الكود المصدري المخصص (`AEL Custom Codebase Philosophy`)

في إطار التزامنا التام ببناء **"موسوعة AEL لعلوم الحاسب" (`AEL CS Encyclopedia`)** بروح سيادية مستقلة، قررنا عدم الاكتفاء بالأكواد القياسية التي استعرضها ديفيد مالان في `src1` (مثل `hello.c` و `agree.c`).

لقد قمنا بتصميم وتأسيس مكتبة برمجية خاصة بنا في مجلد **`ael_src1`**، تتفوق معمارياً على الأكواد القياسية وتقدم للطالب تطبيقات هندسية وعلمية متطورة مكتوبة بلغة `C` الصارمة.

```text
===================================================================================
                  AEL CUSTOM C CODEBASE (ael_src1)
===================================================================================

  [ Week 1 C / ael_src1 ]
   │
   ├─► 🔐 ael_system_auth.c         ──> Zero-Trust Authentication & CLI (argc/argv)
   ├─► 🧮 ael_matrix_calculator.c   ──> 2D Array Stack Allocation & O(N^3) Math
   ├─► 🌌 ael_stellar_orbit.c       ──> Orbital Physics, <math.h> & Double Precision
   └─► 📦 ael_data_compressor.c     ──> Run-Length Encoding (RLE) Memory Compression

===================================================================================
```

---

## 🔬 تشريح الملفات البرمجية الأربعة

### 1. نظام المصادقة الأمنية (`ael_system_auth.c`)
*   **المهمة المعمارية:** يحاكي نظام تسجيل دخول طرفي متقدم للشركات.
*   **المفاهيم المطبقة:**
    *   **معاملات سطر الأوامر (`argc / argv`)**: التحقق الصارم من إدخال اسم المستخدم كمعامل تشغيل إلزامي `argv[1]`.
    *   **حسابات السلاسل النصية (`String Iteration`)**: فحص تطابق كلمة المرور رمزاً تلو الآخر باستخدام التكرار `for` ودالة `strlen`.
    *   **تشفير الإزاحة البسيط (`ASCII Shifting`)**: توليد رمز دخول آمن عبر زيادة القيمة الحسابية لكل حرف بمقدار `+5`، مما يعزز فهم المبرمج لجدول `ASCII`.

### 2. محرك ضرب المصفوفات (`ael_matrix_calculator.c`)
*   **المهمة المعمارية:** يوضح كيفية التعامل مع الشبكات ثنائية الأبعاد في الذاكرة.
*   **المفاهيم المطبقة:**
    *   **المصفوفات ثنائية الأبعاد (`2D Arrays`)**: تخصيص مصفوفات ثابتة في ذاكرة التكدس (`Stack Memory`) بحجم `3x3`.
    *   **الحلقات التكرارية المتداخلة الثلاثية (`Triple Nested Loops`)**: تطبيق الخوارزمية الرياضية القياسية `O(N^3)` لحساب جداء مصفوفتين وتنسيق طباعتها على الشاشة.

### 3. محاكي مدارات الكواكب (`ael_stellar_orbit.c`)
*   **المهمة المعمارية:** تطبيق فيزيائي وعلمي فائق الدقة لحساب سرعة الإفلات الكوكبية (`Escape Velocity`).
*   **المفاهيم المطبقة:**
    *   **مكتبة الرياضيات القياسية (`<math.h>`)**: استدعاء دالة الجذر التربيعي `sqrt`.
    *   **حدود البيانات الدقيقة (`Double & Exponents`)**: استخدام صيغ الأسس العلمية `5.9722e24` والتعامل مع الأعداد الكسرية الضخمة دون تعريض النظام لطفح الذاكرة (`Overflow`).

### 4. خوارزمية ضغط البيانات (`ael_data_compressor.c`)
*   **المهمة المعمارية:** ضغط النصوص والخانات المتكررة لتقليل حجم الذاكرة المستهلكة.
*   **المفاهيم المطبقة:**
    *   **خوارزمية `Run-Length Encoding (RLE)`**: تتبع الحروف المتتالية وعدها (مثال: تحويل `AAAAABBBCCDAA` إلى `A5B3C2D1A2`).
    *   **التحكم في مؤشرات الفحص (`Buffer Peek`)**: قراءة العنصر التتالي `input[i + 1]` بشكل آمن مع منع تجاوز حدود المصفوفة (`Buffer Overrun`).

---

## ⚙️ تعليمات التجميع والتشغيل الجنائي

لتجميع هذه الصروح البرمجية، توجه عبر سطر الأوامر إلى مجلد `ael_src1` واستخدم مترجم `clang` أو أمر `make`:

```bash
# 1. Compile & run System Authentication
$ make ael_system_auth
$ ./ael_system_auth Ayman_Elmasry

# 2. Compile & run Matrix Calculator
$ make ael_matrix_calculator
$ ./ael_matrix_calculator

# 3. Compile & run Stellar Orbit (Link math library -lm if required by clang)
$ make ael_stellar_orbit
$ ./ael_stellar_orbit

# 4. Compile & run Data Compressor
$ make ael_data_compressor
$ ./ael_data_compressor
```

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
