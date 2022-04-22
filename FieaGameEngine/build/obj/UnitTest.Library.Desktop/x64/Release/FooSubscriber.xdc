<?xml version="1.0"?><doc>
<members>
<member name="M:FieaGameEngine.IEventSubscriber.Notify(FieaGameEngine.EventPublisher!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="false" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\Library.Shared\IEventSubscriber.h" line="11">
<summary>
Pure virtual method that accepts the address of an EventPublisher (the actual argument will be the event itself). 
Handlers will use the RTTI interface to verify the event type.
</summary>
<param name=""></param>
</member>
<member name="M:FieaGameEngine.EventPublisher.Deliver" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\Library.Shared\EventPublisher.h" line="19">
<summary>
Notify all subscribers of this event.
</summary>
</member>
<member name="M:FieaGameEngine.EventPublisher.#ctor(FieaGameEngine.Vector&lt;std.reference_wrapper&lt;FieaGameEngine.IEventSubscriber&gt;&gt;*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\Library.Shared\EventPublisher.h" line="25">
<summary>
Constructor
</summary>
<param name="listofSubscribers">Derived Event will pass a refernce to its Vector of EventPublihserk</param>
</member>
<member name="F:FieaGameEngine.EventPublisher._listOfSubscribers" decl="false" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\Library.Shared\EventPublisher.h" line="31">
<summary>
Contains references to subscribers
</summary>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.#ctor" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="17">
<summary>
Initializes the Foo object with default values on declaration.
</summary>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.#ctor(UnitTestLibraryDesktop.Foo!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="22">
<summary>
Copy constructor that initializes the Foo object with another Foo object.
</summary>
<param name="right">The Foo object that is used to initialize the calling Foo object.</param>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.#ctor(System.Int32)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="28">
<summary>
Initializes the Foo object with the value in parameter.
</summary>
<param name="data">An int32_t that initializes the Foo class member variable _data.</param>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.Data" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="36">
<summary>
Returns a reference to member variable _data in a Foo object.
</summary>
<returns>A reference to the member variable _data in the Foo object.</returns>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.op_Assign(UnitTestLibraryDesktop.Foo!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="42">
<summary>
Overloading the assignment operator so that a Foo object can be assigned to another Foo object.
If the parameter is a reference to itself then the function returns a reference to itself.
</summary>
<param name="right">The Foo object that _data is being copied from.</param>
<returns>A reference to itself.</returns>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.op_Equality(UnitTestLibraryDesktop.Foo!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="50">
<summary>
Overloading the equals operator so that a Foo object can be compared against another Foo object.
Performs a comparison of memory location to see if referencing the same object.
Performs a comparison of value to see if they are referencing equivalent values.
</summary>
<param name="right">The Foo object that it is comparing against.</param>
<returns>TRUE if contents are equal. FALSE if contents are not equal.</returns>
</member>
<member name="M:UnitTestLibraryDesktop.Foo.op_Inequality(UnitTestLibraryDesktop.Foo!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="59">
<summary>
Overloading the not equals operator so that a Foo object can be compared against another Foo object.
Calls the operator== and passes the Foo object. Returns the inverse of the result.
</summary>
<param name="right">The Foo object that it is comparing against.</param>
<returns>TRUE if contents are not equal. FALSE if contents are equal.</returns>
</member>
<member name="T:UnitTestLibraryDesktop.FooEqualityFunctor" decl="false" source="C:\Dev\depot\Students\CH18\Alexander.Soto\Prog2\FieaGameEngine\source\UnitTest.Library.Desktop\Foo.h" line="73">
<summary>
Functor that takes two Foo's as parameters and determines if they are equal.
</summary>
</member>
</members>
</doc>