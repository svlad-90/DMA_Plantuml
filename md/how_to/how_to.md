[**Go to the previous page**](../../README.md)

----

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
- From the other side, you can't split the definition of metadata for the
classes and interfaces. There should be only one definition of entity with
its unique name within a package. In case of multiple definitions of an
element the "last win" strategy will be applied. So you can get partial
data ☺
- both class and interface do have: members, methods, inheritance and dependencies
- All parameters of the macro definitions should be placed without the
quotes. Stringification will be done inside the macro definitions
- Concept does contain a primitive type check mechanism. Refer to X_CHECKED
macro definitions. It's usage is optional, so, if needed, you can add to
your design even non-yet-existing classes.
- It is better to specify class names with namespaces in order to avoid
collisions between the entities. E.g. dependencies and inheritance lookup
is using names without considering name of the packages. Thus, if you will
have the package_1::class_1 and package_2::class_1, then dependency to
class_1 might lead to ambiguation, when random data will be chosen.
- All internal search functionality is CASE SENSITIVE. Thus, metadata
declarations should also consider this. Only Creator::findPackagesByName
method does a case-insensitive search of packages.
- Be aware, that you will need a graphwiz installed in order to get plantuml
class diagrams. It is better to install version 2.38 of it, as plantuml
documentation states, that currently it is the most compatible release.
- There is NO support of the nested packages, interfaces or classes. Only
package->class and package->interface folding.
- keywords virtual, override and "=0" should NOT be added together with
the method's definition. They will be added implicitly.
- Concept supports usage of template classes. But be aware, that X_CHECKED
macro definitions will not fully work with it. TClass<int> might work, while
TClass<T> ( where T is abstract template parameter ) definitely won't.
- Concept supports declaration of singletone and abstract classes

### Step 3 ( mandatory ). Somewhere in your code ( in non-global section ) call:

```
DMA::PlantUML::Creator::getInstance().initialize();
```

That will finish initialization of the model.

### Step 4. To obtain the full class diagram of your application, do the following thing in your source code:

```
auto result = DMA::PlantUML::Creator::getInstance().exportAllPackageClassDiagrams("/target_folder/", true);
// Second parameter of the "exportAllPackageClassDiagrams" method specifies whether to exclude package dependencies or not.

for(const auto& resultItem: result)
{
    if(false == resultItem.second.bIsSuccessful)
    {
         // log failure
    }
    else
    {
        // log success
    }
}
```

### Step 5 ( mandatory ). Add the following define to your build:

> PUML_ENABLED

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

For more complex examples, please, visit [this project](https://github.com/svlad-90/DLT-Message-Analyzer).

It's class-diagrams documentation is fully based on usage of this library: [section of DLT-Message-Analyzer's generated UML class-diagrams](https://github.com/svlad-90/DLT-Message-Analyzer/blob/master/md/dev_docs/dev_docs.md).

Search for usage of this API within that repository, and you will get how to
deal with it.

----

[**Go to the previous page**](../../README.md)