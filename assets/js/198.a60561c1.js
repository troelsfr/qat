(window.webpackJsonp=window.webpackJsonp||[]).push([[198],{580:function(e,t,a){"use strict";a.r(t);var r=a(54),o=Object(r.a)({},(function(){var e=this,t=e.$createElement,a=e._self._c||t;return a("ContentSlotsDistributor",{attrs:{"slot-key":e.$parent.slotKey}},[a("h1",{attrs:{id:"llvm"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#llvm"}},[e._v("#")]),e._v(" llvm")]),e._v(" "),a("p",[a("a",{attrs:{href:"#detailed-description"}},[e._v("More...")])]),e._v(" "),a("h2",{attrs:{id:"detailed-description"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#detailed-description"}},[e._v("#")]),e._v(" Detailed Description")]),e._v(" "),a("p",[e._v("QIR Adaptor Tool (QAT)")]),e._v(" "),a("p",[e._v("QAT is a tool that helps the enduser to easily build and use new profiles. The tool provides a commandline interface which is configurable through YAML files to validate a specific QIR profile and generate a QIR profile compatible IR from a generic IR.")]),e._v(" "),a("p",[e._v("The tool itself make use of LLVM passes to perform analysis and transformations of the supplied IR. These transfornations are described through high-level tasks such as "),a("code",[e._v("useStaticQubitArrayAllocation")]),e._v(".")]),e._v(" "),a("p",[e._v("To provide an overview of the structure of this tool, we here provide a diagram showing the relation between different instances in the program:")]),e._v(" "),a("p",[e._v('┌ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ User input │ │ "Use" relation └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ▼ │ argc, argv ▼ ─ ─▶ "Produce" relation ┌──────────────────────────────┐ │ ParameterParser │◀─┐ Setup arguments └──────────────────────────────┘ │ Load config │ │ ▼ │ ┌──────────────────────────────┐ │ ┌──────────────────────────────────┐ │ ConfigurationManager │──┘ ┌ ─ ─ ─▶│ Ruleset │ └──────────────────────────────┘ └──────────────────────────────────┘ Provide config │ │ │ Rules for ▼ ▼ transformation ┌───────────────────────────────┐─ ─ ─ ┘ ┌──────────────────────────────────┐ │ ProfileGenerator │─ ─ ─ ─ ─ ─ ─▶│ TransformationRulesPass │ └───────────────────────────────┘ └──────────────────────────────────┘ │ LLVM module ▼ pass ┌ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┌──────────────────────────────────┐ Output │◀─ ─ ─ ─ ─ ─ ─ ┤ QAT / LLVM Module Pass Manager │ └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ stdout └──────────────────────────────────┘')]),e._v(" "),a("hr"),e._v(" "),a("p",[e._v("Updated on 16 November 2021 at 10:53:54 UTC")])])}),[],!1,null,null,null);t.default=o.exports}}]);