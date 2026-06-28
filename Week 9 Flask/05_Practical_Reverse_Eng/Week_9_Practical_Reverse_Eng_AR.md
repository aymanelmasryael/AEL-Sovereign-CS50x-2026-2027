# AEL CS Encyclopedia™ – Week 9: Practical Reverse Engineering (AR)

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
  "domain": "Week 9 (Flask): MVC Frameworks, Jinja Templating & Session Telemetry",
  "document_type": "05_Practical_Reverse_Eng (AR)",
  "methodology": "8-Stage Sub-Silicon Execution Paradigm",
  "execution_state": "Active Master Verification"
}
```

---

## 🔓 الهندسة العكسية التطبيقية: تشريح أنظمة التسجيل، الجلسات، وعمليات السداد (`froshims`, `login`, `store`)

في هذا القسم، نُجري تفكيكاً هندسياً عكسياً للشفرات التقليدية المقدمة في CS50 لنستخلص الثغرات ونقاط الضعف المعمارية ونعيد صياغتها وفق معايير المنظومة السيادية الفائقة.

### 1. تشريح تطبيق `froshims`: التحقق المزدوج (`Dual Validation Matrix`)
في نماذج `froshims`، تبرز المخاطر المعمارية للاعتماد الحصري على التحقق من صحة البيانات عبر الواجهة الأمامية (`Client-Side Validation`).
*   **ثغرة التلاعب بالـ DOM**: يمكن لأي مستخدم متمرس فتح أدوات المطور (`Inspect Element`) وحذف قيود `HTML` (مثل `required` أو خيارات الـ `select`).
*   **الهندسة المعكوسة للحل السيادي**: فرض التحقق الأمني على الخادم (`Server-Side Validation`) كجدار حماية نهائي:
```python
@app.route("/register", methods=["POST"])
def register():
    sport = request.form.get("sport")
    if not sport or sport not in SPORTS_LIST:
        return render_template("ael_error.html", msg="Sovereign Security Exception: Unauthorized Payload"), 400
```

### 2. تشريح تطبيق `login`: الجلسات وتدميرها (`Session Demolition Matrix`)
يكشف تتبع ملفات `login` أن عملية تسجيل الدخول ليست سوى حقن قيمة في قاموس `session`، بينما تسجيل الخروج يتطلب التطهير الشامل للذاكرة:
```python
@app.route("/logout")
def logout():
    session.clear() # Purges thread-local cryptographic proxy
    return redirect("/login")
```

### 3. تشريح تطبيق `store`: تأمين سلة المشتريات (`Cart Concurrency Vault`)
في تطبيق `store`، يتم تخزين قائمة المنتجات في الجلسة `session["cart"]`. نلاحظ هندسياً أن الجلسة تحافظ على عزل البيانات بين المتسوقين، بحيث لا تظهر مشتريات العميل (أ) في سلة العميل (ب)، حتى لو استخدم كلاهما الخادم في نفس الميلي ثانية.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**
