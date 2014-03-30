/*
 * queue.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */

#ifndef _IRRLIGHT_ACTION_EVENT_QUEUE_HPP_
#define _IRRLIGHT_ACTION_EVENT_QUEUE_HPP_


namespace irrlight{

//This isn't used anywhere, it's just a nice event queue class
typedef boost::function <void ()> Event;
class IsometricActionServiceEventQueue{
	private:
		std::vector<Event> queue;
		boost::recursive_mutex queueMutex;
		static void tick(IsoActor * actor, AIsometricAction * action){
			action->tick(actor);
		}
	protected:
		const std::vector<Event> PopEvents (){
			boost::recursive_mutex::scoped_lock lock (queueMutex);
			const std::vector<Event> eventsCopy = queue;
			queue.clear ();
			return eventsCopy;
		}
	public:
		const bool PlayOutEvents (){
			// The copy is there to ensure there are no deadlocks.
			const std::vector<Event> queueCopy = PopEvents ();
			BOOST_FOREACH (const Event& e, queueCopy){
				boost::thread thrd(e);
				//Sleep (0);
			}
			return queueCopy.size () > 0;
		}

		void AddEvent (IsoActor * actor, AIsometricAction * action) /*const Event& event)*/{
			boost::recursive_mutex::scoped_lock lock (queueMutex);
			//queue.push_back (boost::bind(&IsometricActionServiceEventQueue::tick, actor, action));
			boost::thread thrd(boost::bind(&IsometricActionServiceEventQueue::tick, actor, action));
		}
};

};
#endif /* _IRRLIGHT_ACTION_EVENT_QUEUE_HPP_ */
