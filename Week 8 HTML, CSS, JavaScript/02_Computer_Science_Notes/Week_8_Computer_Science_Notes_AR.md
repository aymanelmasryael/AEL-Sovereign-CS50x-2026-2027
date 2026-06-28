# AEL CS Encyclopedia™ – Week 8: Computer Science Notes (AR)

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
  "domain": "Week 8 (HTML, CSS, JavaScript): DOM Architecture, Event Loops & UI Glassmorphism",
  "document_type": "02_Computer_Science_Notes (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🧠 ملاحظات علوم الحاسب: البنية الشجرية، وحلقة الأحداث (Event Loop)

### 1. نموذج شجرة الوثيقة (`DOM Tree & Node Graph`)
*   **هيكل شجرة الكائنات (`Tree Data Structure`)**: المتصفح يقرأ وسوم الـ `HTML` ويبني شجرة هرمية من الـ `Nodes`. الكائن الجذري هو `document`، ومنه تتفرع `html` ثم `head` و `body`.
*   **التشعب في الذاكرة (`DOM in Memory`)**: كل عقدة (`Node`) في هذه الشجرة هي عبارة عن كائن ديناميكي يحتوي على خصائص (`Properties`) ودوال (`Methods`) قابلة للتعديل والوصول المباشر في الذاكرة.

```text
===================================================================================
                   DOCUMENT OBJECT MODEL (DOM) TREE HIERARCHY
===================================================================================

                        [ document ]
                             │
                         [ html ]
                        /        \
                    [ head ]    [ body ]
                                 /    \
                           [ div ]    [ section ]
                            /   \          │
                       [ h1 ]  [ p ]    [ button ]

===================================================================================
```

### 2. حلقة الأحداث غير المتزامنة (`The JavaScript Event Loop`)
لغة `JavaScript` تعمل تقليدياً كخيط واحد (`Single-Threaded`)، ولكنها تتعامل مع العمليات الثقيلة وغير المتزامنة من خلال بنية متكاملة:
*   **مكدس الاستدعاء (`Call Stack`)**: مكان تنفيذ الشفرات والدوال الحالية أولاً بأول (`LIFO`).
*   **واجهات الويب (`Web APIs`)**: تُدار من المتصفح لمعالجة الـ `setTimeout`، وطلبات الـ `fetch`، وأحداث النقر `EventListeners`.
*   **طابور المهام (`Task Queue`)**: عندما تنتهي العملية من واجهات الويب، تدخل طابور المهام (`FIFO`).
*   **حلقة الأحداث (`Event Loop`)**: المراقب الدائم الذي ينقل الدالة من طابور المهام إلى مكدس الاستدعاء متى أصبح فارغاً.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
