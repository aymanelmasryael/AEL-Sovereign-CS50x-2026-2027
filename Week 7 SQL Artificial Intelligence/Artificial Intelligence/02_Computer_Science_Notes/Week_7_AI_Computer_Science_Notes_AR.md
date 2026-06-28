# AEL CS Encyclopedia™ – Week 7 AI Subsystem: Computer Science Notes (AR)

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
  "domain": "Week 7 (AI Subsystem): Tokenization, Embedding Vectors & Attention Mechanisms",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🧠 مذكرات علوم الحاسب: التمكين الرياضي للذكاء الاصطناعي (Tokenization & Vectors)

لفهم عمل النماذج اللغوية الكبيرة (`LLMs`) بعيداً عن السحر السطحي، يجب النظر إلى أسسها الرياضية الصارمة وكيفية معالجتها للبيانات داخل الفضاء المتجهي (`Vector Space`).

### 1. تقطيع النصوص والترميز (`Tokenization`)
لا تفهم الشبكات العصبية الكلمات كنصوص بشرية (`ASCII / UTF-8 strings`).
*   **التفكيك إلى رموز (`Tokens`)**: يتم تقسيم الكلمات أو أجزاء الكلمات إلى أرقام تعريفية ثابتة (`IDs`). الكلمة الواحدة قد تمثل رمزاً واحداً أو تُقسم إلى مقاطع (مثلاً `Tokenization` قد تصبح `Token` و `ization`).
*   **نافذة السياق (`Context Window`)**: تمثل الحد الأقصى لعدد الرموز (`Tokens`) التي يمكن للنموذج استيعابها في الذاكرة الحية لحساب التنبؤ القادم.

### 2. التضمين المتجهي والفضاء الدلالي (`Embedding Vectors`)
*   **التمثيل متعدد الأبعاد (`High-Dimensional Vectors`)**: يتم تحويل كل رمز (`Token`) إلى قائمة من الأرقام العشرية (تتجاوز غالباً 1536 أو 4096 بعداً).
*   **المسافة الدلالية (`Semantic Euclidean & Cosine Similarity`)**: الكلمات ذات المعاني المتقاربة (مثل `King` و `Queen` أو `Cat` و `Dog`) تقع في نقاط متقاربة هندسياً داخل هذا الفضاء المتجهي الشاسع.

```text
===================================================================================
                   HIGH-DIMENSIONAL EMBEDDING VECTOR SPACE
===================================================================================

       [ Cat ] (0.25, 0.89, 0.12)  <── Close Cosine Distance ──> [ Dog ] (0.28, 0.85, 0.15)
       
                                   <── Massive Distance ────> [ Car ] (0.91, 0.05, 0.88)

===================================================================================
```

### 3. آلية الانتباه الذاتي (`Self-Attention Mechanism`)
الابتكار الثوري في معمارية `Transformers` (`Attention Is All You Need`).
*   تسمح للنموذج بحساب مصفوفات الارتباط بين كل كلمة وجميع الكلمات الأخرى في النص، مما يتيح له تحديد الضمائر وسياق الكلمات المتشابهة بدقة متناهية.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
