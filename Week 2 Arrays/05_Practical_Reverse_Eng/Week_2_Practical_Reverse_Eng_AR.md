# AEL CS Encyclopedia™ – Week 2 Arrays: Practical Reverse Engineering (AR)

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
  "domain": "Week 2: Scrabble, Readability & Ciphers",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔬 المحور الأول: الهندسة العكسية للعبة سكرابل (`Scrabble Engine Deconstruction`)

### 1. الهدف المعماري للمشروع (`Architectural Objective`)
البرنامج يطلب من لاعبين (`Player 1` و `Player 2`) إدخال كلمتين، ثم يحسب مجموع نقاط كل كلمة استناداً إلى جدول نقاط محدد لكل حرف في الأبجدية الإنجليزية، ليعلن في النهاية عن الفائز.

```text
===================================================================================
             SCRABBLE REVERSE ENGINEERING PIPELINE
===================================================================================

  [ Input String (Word) ] ──> [ Loop Character by Character (strlen) ]
                                      │
                                      ▼
             [ isalpha() ? Convert to Uppercase (toupper) ]
                                      │
                                      ▼
             [ Index Calculation: char - 'A' (0 to 25) ]
                                      │
                                      ▼
             [ Lookup in POINTS[] Matrix ] ──> [ Accumulate Total Score ]

===================================================================================
```

### 2. مصفوفة النقاط الثابتة (`The POINTS Matrix`)
يتم تخزين النقاط في مصفوفة ثابتة من 26 عنصراً:
```c
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
```
* **آلية الفهرسة الجنائية**: للحصول على نقاط الحرف `C` (والذي قيمته في ASCII هي 67)، نقوم بالعملية الحسابية `67 - 65 ('A') = 2`. بفحص `POINTS[2]` نحصل على القيمة `3`.

---

## 🧠 المحور الثاني: الهندسة العكسية لمشروع قياس القراءة (`Readability Engine`)

### 1. خوارزمية فحص النصوص المدمجة (`Integrated Text Scanner`)
بدلاً من إجراء 3 حلقات تكرارية منفصلة لحساب الحروف، الكلمات، والجمل، نقوم بتصميم حلقة واحدة عالية الكفاءة تفحص النص حرفاً بحرف:

```text
===================================================================================
             READABILITY PARSER LOGIC TREE
===================================================================================

                [ Character Scanner (s[i]) ]
                 ├── isalpha(s[i]) ──> letters++
                 ├── isspace(s[i]) ──> words++ (Initialize words at 1)
                 └── s[i] == '.' || s[i] == '!' || s[i] == '?' ──> sentences++

===================================================================================
```

### 2. تطبيق معادلة كولمان-ليو الصارمة (`Coleman-Liau Application`)
```c
float L = ((float) letters / words) * 100;
float S = ((float) sentences / words) * 100;
int index = round(0.0588 * L - 0.296 * S - 15.8);
```
* **التحقق من المخرجات (`Output Verification`)**:
  * إذا كان `index < 1`، نطبع `Before Grade 1`.
  * إذا كان `index >= 16`، نطبع `Grade 16+`.
  * بخلاف ذلك، نطبع `Grade X`.

---

## 🧬 المحور الثالث: الهندسة العكسية لخوارزميات التشفير (`Caesar & Substitution`)

### 1. تشريح مشروع قيصر (`Caesar Deconstruction`)
* **تأمين سطر الأوامر (`CLI Validation`)**: يجب التحقق من أن `argc == 2`، وأن كل حرف في `argv[1]` هو رقم باستخدام دالة `isdigit()`.
* **التحويل إلى قيمة رقمية**: تحويل السلسلة النصية `argv[1]` إلى عدد صحيح باستخدام `atoi()`.
* **الحفاظ على حالة الحروف**: عند الإزاحة، يتم الاحتفاظ بالحروف الكبيرة `isupper` والصغيرة `islower` كما هي، مع تجاهل الرموز والمسافات.

### 2. تشريح مشروع الاستبدال (`Substitution Deconstruction`)
* **فحص المفتاح الجنائي (`Key Validation`)**: التحقق من أن `argv[1]` يحتوي على 26 حرفاً بالضبط (`strlen == 26`)، وأن جميعها حروف أبجدية، وأنه لا توجد حروف مكررة (عبر مصفوفة تتبع تردد الحروف `Frequency Array`).
* **فهرسة الاستبدال**: استبدال الحرف الأصلي بالحرف المقابل في المفتاح بناءً على ترتيبه الأبجدي `char - 'A'`.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
