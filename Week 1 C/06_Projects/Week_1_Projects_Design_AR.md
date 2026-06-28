# Projects Architectural Design: Week 1 - C (Mario, Cash & Credit Systems)

---

## 🏛️ تشريح المشاريع التطبيقية (Classic CS50 Problem Sets)

تمثل مشاريع الأسبوع الأول في لغة `C` أول احتكاك حقيقي للطالب بهندسة البرمجيات القائمة على حل المشكلات (`Problem-Solving Architecture`). في هذا المجلد، نقوم بتشريح الأساليب الخوارزمية والتصميمية لثلاثة مشاريع كلاسيكية في مسار `CS50x`:

```text
===================================================================================
                       WEEK 1 C - PROJECTS ARCHITECTURE
===================================================================================

  ┌────────────────────────────────────────────────────────┐
  │ 1. MARIO (Less & More Comfortable)                     │ ──> Nested Loops & Alignment
  ├────────────────────────────────────────────────────────┤
  │ 2. CASH (Less Comfortable)                             │ ──> The Greedy Algorithm
  ├────────────────────────────────────────────────────────┤
  │ 3. CREDIT (More Comfortable)                           │ ──> Luhn's Algorithm & Modulo Math
  └────────────────────────────────────────────────────────┘

===================================================================================
```

---

## 🧱 1. مشروع هرم ماريو (`Mario - Less & More Comfortable`)

يستلهم هذا المشروع من لعبة `Super Mario Bros` لبناء أهرامات متدرجة من كتل الطوب `#` في نافذة سطر الأوامر. الهدف الهندسي هو إتقان الحلقات التكرارية المتداخلة (`Nested Loops`) وإجبار المستخدم على إدخال قيم صحيحة (`Input Validation`).

### هرم `Mario - Less Comfortable` (الهرم الأيمن)
*   **المتطلبات:** بناء هرم واحد يرتفع لليمين، بارتفاع يتراوح بين 1 و 8.
*   **التصميم المعماري:**
```text
   #  (3 spaces, 1 hash)
  ##  (2 spaces, 2 hashes)
 ###  (1 space,  3 hashes)
####  (0 spaces, 4 hashes)
```
*   **القواعد الحسابية للحلقات:**
    *   لأي سطر `i` (من `0` إلى `height - 1`)، عدد المسافات `spaces = height - i - 1`.
    *   عدد الطوب `hashes = i + 1`.

### هرم `Mario - More Comfortable` (الهرم المزدوج)
*   **المتطلبات:** بناء هرمين متقابلين تفصل بينهما مسافتان ثابتتان `  `.
*   **التصميم المعماري:**
```text
   #  #
  ##  ##
 ###  ###
####  ####
```
*   **تنفيذ الشفرة الهندسية (`mario.c`):**
```c
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // 1. Input Validation (1 to 8)
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // 2. Master Loop for Rows
    for (int i = 0; i < height; i++)
    {
        // Print leading spaces
        for (int spaces = 0; spaces < height - i - 1; spaces++)
        {
            printf(" ");
        }

        // Print left pyramid hashes
        for (int hashes = 0; hashes <= i; hashes++)
        {
            printf("#");
        }

        // Print middle static gap
        printf("  ");

        // Print right pyramid hashes
        for (int hashes = 0; hashes <= i; hashes++)
        {
            printf("#");
        }

        // Move to next row
        printf("\n");
    }
}
```

---

## 🪙 2. مشروع آلة الصرافة وخوارزمية النهم (`Cash - The Greedy Algorithm`)

في مشروع `Cash`، يطلب من المبرمج تصميم نظام يحسب أقل عدد ممكن من العملات المعدنية اللازمة لإعطاء الباقي للعميل.

```text
===================================================================================
                       THE GREEDY ALGORITHM PIPELINE
===================================================================================

  [ Cents Input: e.g., 68¢ ]
            │
            ├─► Quarters (25¢) ──> 68 / 25 = 2 (Remainder: 18¢)
            ├─► Dimes (10¢)    ──> 18 / 10 = 1 (Remainder: 8¢)
            ├─► Nickels (5¢)   ──> 8 / 5   = 1 (Remainder: 3¢)
            └─► Pennies (1¢)   ──> 3 / 1   = 3 (Remainder: 0¢)
            
  [ Total Minimum Coins = 2 + 1 + 1 + 3 = 7 Coins ]

===================================================================================
```

### تشريح خوارزمية النهم (`Greedy Algorithm`):
*   **المفهوم الخوارزمي:** تقوم الخوارزمية الجشعة أو النهمة باختيار "أفضل وأكبر خيار متاح حالياً" في كل مرحلة دون النظر إلى المستقبل. في حالة العملات القياسية الأمريكية (25, 10, 5, 1)، ثبت رياضياً أن هذه الخوارزمية تنتج دائماً الحل الأمثل.
*   **تنفيذ الشفرة الهندسية (`cash.c`):**
```c
#include <cs50.h>
#include <stdio.h>

int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    int quarters = calculate_quarters(cents);
    cents = cents - (quarters * 25);

    int dimes = calculate_dimes(cents);
    cents = cents - (dimes * 10);

    int nickels = calculate_nickels(cents);
    cents = cents - (nickels * 5);

    int pennies = calculate_pennies(cents);
    cents = cents - (pennies * 1);

    int coins = quarters + dimes + nickels + pennies;
    printf("%i\n", coins);
}

int calculate_quarters(int cents) { return cents / 25; }
int calculate_dimes(int cents)    { return cents / 10; }
int calculate_nickels(int cents)  { return cents / 5; }
int calculate_pennies(int cents)  { return cents / 1; }
```

---

## 💳 3. مشروع التحقق من بطاقات الائتمان (`Credit - Luhn's Algorithm`)

مشروع `Credit` هو تحدي معماري متقدم يتطلب التحقق من صحة أرقام بطاقات الائتمان (`Visa, MasterCard, American Express`) باستخدام **خوارزمية لونه (`Luhn's Algorithm`)**.

```text
===================================================================================
                       LUHN'S CARD VERIFICATION ALGORITHM
===================================================================================

  [ Card Number: e.g., 4 0 1 2 8 8 8 8 8 8 8 8 8 8 8 3 ] (Length 16, Starts 4 -> Visa)
            │
            ├─► 1. Multiply every second digit from right by 2 (Add product digits)
            ├─► 2. Add sum of digits that weren't multiplied
            └─► 3. If total modulo 10 == 0, Card is VALID.

===================================================================================
```

### 1. تصنيف البطاقات حسب المعايير الدولية:
*   **American Express (`AMEX`):** الطول `15` رقماً، وتبدأ بالرقمين `34` أو `37`.
*   **MasterCard:** الطول `16` رقماً، وتبدأ بالأرقام من `51` إلى `55`.
*   **Visa:** الطول `13` أو `16` رقماً، وتبدأ بالرقم `4`.

### 2. تشريح خوارزمية لونه (`Luhn's Algorithm`):
1.  **الخطوة الأولى:** البدء من الرقم الأخير، وتجاوز الرقم الأول نحو اليسار، وضرب كل رقم ثاني في `2`.
    *   *ملاحظة خطيرة:* إذا ناتج الضرب تشكل من رقمين (مثل `6 * 2 = 12`)، لا نجمع `12`، بل نجمع أرقام الناتج `1 + 2 = 3`.
2.  **الخطوة الثانية:** جمع كافة الأرقام الأخرى التي لم يتم ضربها.
3.  **الخطوة الثالثة:** جمع الناتجين. إذا كان المجموع الكلي ينتهي بصفر (أي `total_sum % 10 == 0`)، فإن بطاقة الائتمان سليمة.

### 3. تقنيات المعالجة الرياضية (`Modulo & Division Math`):
في لغة C، نظراً لعدم إمكانية تحويل الرقم إلى سلسلة نصية بسهولة في البداية، يتم انتزاع الأرقام واحداً تلو الآخر باستخدام باقي القسمة `card % 10`، ثم إزالتها كلياً بالقسمة `card /= 10`. ونظراً لطول رقم البطاقة (حتى 16 رقماً)، يتم استخدام نوع البيانات `long` لتجنب الطفح (`Overflow`).

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
