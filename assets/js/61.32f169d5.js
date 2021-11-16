(window.webpackJsonp=window.webpackJsonp||[]).push([[61],{445:function(s,a,t){"use strict";t.r(a);var e=t(54),n=Object(e.a)({},(function(){var s=this,a=s.$createElement,t=s._self._c||a;return t("ContentSlotsDistributor",{attrs:{"slot-key":s.$parent.slotKey}},[t("h1",{attrs:{id:"developer-faq"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#developer-faq"}},[s._v("#")]),s._v(" Developer FAQ")]),s._v(" "),t("h2",{attrs:{id:"pass-does-not-load"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#pass-does-not-load"}},[s._v("#")]),s._v(" Pass does not load")]),s._v(" "),t("p",[s._v("One error that you may encounter is that an analysis pass does not load with output similar to this:")]),s._v(" "),t("div",{staticClass:"language-sh extra-class"},[t("pre",{pre:!0,attrs:{class:"language-sh"}},[t("code",[s._v("opt -load-pass-plugin "),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v("..")]),s._v("/"),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v("..")]),s._v("/Debug/libQSharpPasses.dylib -enable-debugify  --passes"),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v("=")]),t("span",{pre:!0,attrs:{class:"token string"}},[s._v('"operation-counter"')]),s._v(" -disable-output   classical-program.bc\nFailed to load passes from "),t("span",{pre:!0,attrs:{class:"token string"}},[s._v("'../../Debug/libQSharpPasses.dylib'")]),t("span",{pre:!0,attrs:{class:"token builtin class-name"}},[s._v(".")]),s._v(" Request ignored.\nopt: unknown pass name "),t("span",{pre:!0,attrs:{class:"token string"}},[s._v("'operation-counter'")]),s._v("\n")])])]),t("p",[s._v("This is likely becuase you have forgotten to instantiate static class members. For instance, in the case of an instance of "),t("code",[s._v("llvm::AnalysisInfoMixin")]),s._v(" you are required to have a static member "),t("code",[s._v("Key")]),s._v(":")]),s._v(" "),t("div",{staticClass:"language-cpp extra-class"},[t("pre",{pre:!0,attrs:{class:"language-cpp"}},[t("code",[t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("class")]),s._v(" "),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("COpsCounterPass")]),s._v(" "),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(":")]),s._v("  "),t("span",{pre:!0,attrs:{class:"token base-clause"}},[t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("public")]),s._v(" llvm"),t("span",{pre:!0,attrs:{class:"token double-colon punctuation"}},[s._v("::")]),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("AnalysisInfoMixin")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v("<")]),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("COpsCounterPass")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(">")])]),s._v(" "),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v("{")]),s._v("\n"),t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("private")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(":")]),s._v("\n  "),t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("static")]),s._v(" llvm"),t("span",{pre:!0,attrs:{class:"token double-colon punctuation"}},[s._v("::")]),s._v("AnalysisKey Key"),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v(";")]),s._v(" "),t("span",{pre:!0,attrs:{class:"token comment"}},[s._v("//< REQUIRED by llvm registration")]),s._v("\n  "),t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("friend")]),s._v(" "),t("span",{pre:!0,attrs:{class:"token keyword"}},[s._v("struct")]),s._v(" "),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("llvm")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(":")]),t("span",{pre:!0,attrs:{class:"token base-clause"}},[t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(":")]),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("AnalysisInfoMixin")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v("<")]),t("span",{pre:!0,attrs:{class:"token class-name"}},[s._v("COpsCounterPass")]),t("span",{pre:!0,attrs:{class:"token operator"}},[s._v(">")])]),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v(";")]),s._v("\n"),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v("}")]),t("span",{pre:!0,attrs:{class:"token punctuation"}},[s._v(";")]),s._v("\n")])])]),t("p",[s._v("If you forget to instantiate this variable in your corresponding "),t("code",[s._v(".cpp")]),s._v(" file,")]),s._v(" "),t("div",{staticClass:"language-cpp extra-class"},[t("pre",{pre:!0,attrs:{class:"language-cpp"}},[t("code",[t("span",{pre:!0,attrs:{class:"token comment"}},[s._v("// llvm::AnalysisKey COpsCounterPass::Key; //< Uncomment this line to make everything work")]),s._v("\n")])])]),t("p",[s._v("everything will compile, but the pass will fail to load. There will be no linking errors either.")])])}),[],!1,null,null,null);a.default=n.exports}}]);