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
</members>
</doc>