GTA VI LEGACY CORE — مشروع ASI أصلي بلغة C++

عدّلت ملف المشروع ليقرأ مجلدي inc و lib الموجودين بجانب ملف المشروع مباشرة.
لا تحتاج تعديل المسارات يدوياً.

تأكد أن ترتيب الملفات هكذا:
GTAVI_Legacy_Core_CPP
  inc
    script.h
    natives.h
    (بقية ملفات SDK)
  lib
    ScriptHookV.lib
  src
    GTAVI_Legacy_Core.cpp
  GTAVI_Legacy_Core.vcxproj

البناء:
1) افتح GTAVI_Legacy_Core.vcxproj في Visual Studio.
2) اختر Release من قائمة Configuration.
3) اختر x64 من قائمة Platform.
4) من Build اختر Build Solution.
5) عند نجاح البناء ستجد:
   bin\Release\GTAVI_Legacy_Core.asi

هذا مشروع مصدر C++ وليس ملف ASI مبني مسبقاً.
يحتاج Visual Studio مع Desktop development with C++ وملفات ScriptHookV SDK الصحيحة.
Story Mode فقط؛ لا تستخدمه في GTA Online.

التحكم:
F10 فتح/إغلاق القائمة
Up/Down للتنقل
Enter للتنفيذ
Esc للإغلاق

لإزالة الإضافة بعد البناء: احذف GTAVI_Legacy_Core.asi من مجلد GTA V.
