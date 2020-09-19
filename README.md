# DMA_Plantuml

- Module:   DMA_Plantuml
- Version:  1.0.1
- Author:   Vladyslav Goncharuk ( svlad1990@gmail.com )

----

## Brief

Where am I?

You are on the page of the DMA_Plantuml project - the poor man's C++
to platuml converter

What it does? It allows you to declare a metadata of your classes, which is
then used to produce plantuml class diagrams. 
In other words, with it your application starts to contain data regarding its own design.

## Motivation:

Firstly, as a developer I want documentation of my source code to be located
as near as possible to the code itself.

Secondly, I do not want to use existing tools, which are generating the
diagrams during the build. Integration of such tools usually needs a lot of
efforts to configure them properly, and, at final point, the result diagram
is representing either the whole application ( which is overloaded with
classes ), or only a single class ( which is not informative ).

Thirdly, when user states, that there is an issue in some version of the SW,
I want to be able to get class diagrams from the SW itself, without
referring to a specific base-line in git.

Fourthly, from time to time I want to be able to review and adjust the
design of my application. When I'm doing this, I want to be able to
instantly see the result of code modification in a diagram, without the
need to refer to a thirdparty tools or files.

Fifthly, currently existing plugins for IDE-s, which allow to visulize plantuml
comments in the source code are not sufficient, as I want to review not only
the diagram of a specific single class, but also connections between
multiple classes, interfaces, packages, etc. Current concept supports
filtering out the diagram based on certain package. You are able to request
creation of diagram for the specified package and its nearest connections.

As of now, this concept is manual, but, for sure, later on it can be used as
a core module for a generator, which will produce the metadata macro-
definitions on the fly.

General idea of this concept is to allow you to have UML data ( mainly class
diagrams ) of your app at your fingertips. From my previous experience, the
more overhead you have to create the documentation, the poorer it would be,
mainly due to lack of capacity to support it. With this small lib I try to
have a proper tool, which I will use at least for my own projects ☺

## Where is it actually used?

At least in this project: https://github.com/svlad-90/DLT-Message-Analyzer

## How to use it?

### Step 1 ( optional ). Declare some class hierarchy, for which you will declare the plantuml metadata afterwards.

### Step 2 ( mandatory ). In your cpp(s) add declarations of your classes, using provided macro-based API.

Example:

```
PUML_PACKAGE_BEGIN(test_main)
    PUML_INTERFACE_BEGIN(IInterface)
        PUML_VIRTUAL_METHOD(+, ~IInterface())
        PUML_VIRTUAL_METHOD(+, void virtFunc() )
        PUML_PURE_VIRTUAL_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
    PUML_INTERFACE_END()

    PUML_CLASS_BEGIN(CImpl_Test)
        PUML_INHERITANCE(IInterface, implements)
        PUML_INHERITANCE(IInterfaceHelper, implements)
        PUML_INHERITANCE(UndeclaredClass, extends)
        PUML_METHOD(+, CImpl_Test())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
        PUML_OVERRIDE_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
        PUML_STATIC_METHOD(+, void myStaticMethod() )
        PUML_COMPOSITION_DEPENDENCY(CompositionDependency, 1, 1, contains)
        PUML_AGGREGATION_DEPENDENCY(AggregationDependency, 1, 1, uses)
    PUML_CLASS_END()
PUML_PACKAGE_END()

PUML_PACKAGE_BEGIN(test_helper)

    PUML_INTERFACE_BEGIN(IInterfaceHelper)
    PUML_INTERFACE_END()

    PUML_INTERFACE_BEGIN(IDependency)
    PUML_INTERFACE_END()

    PUML_CLASS_BEGIN(CompositionDependency)
        PUML_INHERITANCE(IDependency, implements)
        PUML_AGGREGATION_DEPENDENCY(ExternalDependency, 1, 1, uses)
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(AggregationDependency)
        PUML_INHERITANCE(IDependency, implements)
        PUML_AGGREGATION_DEPENDENCY(ExternalDependency, 1, 1, uses)
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(CImpl_Helper)
        PUML_INHERITANCE(IInterface, implements)
        PUML_METHOD(+, CImpl_Helper())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
        PUML_OVERRIDE_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
        PUML_STATIC_METHOD(+, void myStaticMethod() )
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(CImpl_HelperExtended)
        PUML_INHERITANCE(CImpl_Helper, extends)
        PUML_METHOD(+, CImpl_HelperExtended())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
    PUML_CLASS_END()

PUML_PACKAGE_END()

PUML_PACKAGE_BEGIN(test_external)
    PUML_CLASS_BEGIN(ExternalDependency)
    PUML_CLASS_END()
PUML_PACKAGE_END()
```

Some rules and tips regarding the macro definitions:
- Package is mandatory. You can not declare anything outside the package.
You can try but you will get a compilcaiton error, as macro definitions are
created to work only in that way. Otherwise produced C++ code won't be valid.
That is my shoutout to the component architecture. Declare packages!
- You can declare the same package in different files. E.g. if you have a
package with dozens of files there is no need to have a seperate cpp in
order to store the whole metadata of the package there. No! Idea is to
declare metadata of the specific entities in their own cpp files. Even if
multiple classes are related to the same package.
- From the other side you can't split the definition of metadata for the
classes and interfaces. There should be only one definition of entity with
its unique name within a package. In case of multiple definitions of an
element the "last win" strategy will be applied. So you can get partial
data ☺
- Class has: methods, inheritance, dependencies
- Interface has: methods, dependencies. No inheritance.
- All parameters of the macro definitions should be placed without the
quotes. Stringification will be done inside the macro definitions
- Concept does NOT contain a type check mechanism. From one side that allows
you to register some garbage. But from the other - it allows to create
design for non-existing classes, which is useful.
- It is better to specify class names with namespaces in order to avoid
collisions between the entities. E.g. dependencies and inheritance are
searched by their names without considering name of the package. Thus, if
you will have the package_1::class_1 and package_2::class_1, then dependency
to class_1 might lead to ambiguation, when random data will be chosen.
- All internal search functionality is CASE SENSITIVE. Thus, metadata
declarations should also consider this. Only Creator::findPackagesByName does
a case-insensitive search.
- Be aware, that you will need a graphwiz installed in order to get plantuml
class diagrams. It is better to install version 2.38 of it.
- There is NO support of the nested packages, interfaces or classes. Only
package->class and package->interface folding.
- keywords virtual, override and "=0" should NOT be added together with
the method's definition. They will be added implicitly.
- Dependencies of class or interface to itself will be ignored.

### Step 3 ( mandatory ). Somewhere in your code ( in non-global section ) call:

DMA::PlantUML::Creator::getInstance().initialize();

That will finish initialization of the model.

### Step 4. To obtain the full class diagram of your application, do the following thing in your source code:

```
auto diagramResult = DMA::PlantUML::Creator::getInstance().getClassDiagram();

if(true == diagramResult.bIsSuccessful)
{
    // do something with diagramResult.diagramContent
}
else
{
    // dump diagramResult.error
}
```

### Step 5. To obtain partial class diagram of the specific package, do the following thing in your source code:

```
auto diagramResult = DMA::PlantUML::Creator::getInstance().getPackageClassDiagram("test");

if(true == diagramResult.bIsSuccessful)
{
    // do something with diagramResult.diagramContent
}
else
{
    // dump diagramResult.error
}
```

### Step 6 ( mandatory ). Add the following define to your build:

> #define PUML_ENABLED

Without it, all macro definitions which you've used will produce nothing.
Sometimes ( in release builds ) this define can be turned off by purpose in
order to avoid runtime overheads.

----

## Example of resulting diagram, which is based on the model from step 2:

```
@startuml

package "test_external" #DDDDDD
{

class "ExternalDependency"
{
}

}

package "test_helper" #DDDDDD
{

class "AggregationDependency"
{
}

class "CImpl_Helper"
{
+ CImpl_Helper()
+ {static} void myStaticMethod()
+ virtual void pureVirtFunc(const int& val1, const int& val2) override
+ virtual void virtFunc() override
}

class "CImpl_HelperExtended"
{
+ CImpl_HelperExtended()
+ virtual void virtFunc() override
}

class "CompositionDependency"
{
}

interface "IDependency"
{
}

interface "IInterfaceHelper"
{
}

}

package "test_main" #DDDDDD
{

class "CImpl_Test"
{
+ CImpl_Test()
+ {static} void myStaticMethod()
+ virtual void pureVirtFunc(const int& val1, const int& val2) override
+ virtual void virtFunc() override
}

interface "IInterface"
{
+ {abstract} virtual void pureVirtFunc(const int& val1, const int& val2) = 0
+ virtual void virtFunc()
+ virtual ~IInterface()
}

}

'====================Inheritance section====================
IDependency <|-- AggregationDependency : implements
IInterface <|-- CImpl_Helper : implements
CImpl_Helper <|-- CImpl_HelperExtended : extends
IDependency <|-- CompositionDependency : implements
IInterface <|-- CImpl_Test : implements
IInterfaceHelper <|-- CImpl_Test : implements
UndeclaredClass <|-- CImpl_Test : extends

'====================Dependencies section====================
AggregationDependency "1" o-- "1" ExternalDependency : uses
CompositionDependency "1" o-- "1" ExternalDependency : uses
CImpl_Test "1" o-- "1" AggregationDependency : uses
CImpl_Test "1" -- "1" CompositionDependency : contains

@enduml
```

----

## How diagram looks like?

### Full diagram

![full_diagram](./md/full_diagram.svg)

### Package diagram with external dependencies

![package_diagram_with_external_dependencies](./md/package_diagram_with_external_dependencies.svg)

### Package diagram without external dependencies

![package_diagram_without_external_dependencies](./md/package_diagram_without_external_dependencies.svg)
