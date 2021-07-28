/**
 * MIT License
 * 
 * Copyright (c) 2021 John Wakley
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Portions of this code are subject to:
 * 
 * Copyright (c) 2014 Sean Farrell
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 * Sources:
 * https://gist.github.com/rioki/1290004d7505380f2b1d
 * https://gist.github.com/martinfinke/a636dcddbcf112344b59
 * 
**/

#pragma once

#include <functional>
#include <memory>
#include <map>
#include <list>
#include <string>
#include <iostream>

namespace ezi
{
    class EventEmitter
    {
    public:
        EventEmitter() {}
        ~EventEmitter() {}

        template <typename... Args>
        std::string addListener(const std::string &name, std::function<void(Args...)> callback)
        {
            std::string listener_name = name + "_" + std::to_string(++lastListener);
            listeners.insert(std::make_pair(name, std::make_shared<Listener<Args...>>(listener_name, callback)));
            return listener_name;
        }

        std::string addListener(const std::string &name, std::function<void()> callback)
        {
            std::string listener_name = name + "_" + std::to_string(++lastListener);
            listeners.insert(std::make_pair(name, std::make_shared<Listener<>>(listener_name, callback)));
            return listener_name;
        }

        template <typename Lambda>
        std::string addListener(const std::string &name, Lambda callback)
        {
            return addListener(name, make_function(callback));
        }

        void removeListener(const std::string &listener_name)
        {
            auto iterator = std::find_if(listeners.begin(), listeners.end(), [&](std::pair<std::string, std::shared_ptr<ListenerBase>> p)
                                         { return p.second->name == listener_name; });

            if (iterator != listeners.end())
            {
                listeners.erase(iterator);
            }
            else
            {
                throw std::invalid_argument("EventEmitter::removeListener: invalid listener name");
            }
        }

        template <typename... Args>
        void emit(const std::string &name, Args... args)
        {
            std::list<std::shared_ptr<Listener<Args...>>> handlers;
            auto range = listeners.equal_range(name);
            handlers.resize(std::distance(range.first, range.second));

            std::transform(range.first, range.second, handlers.begin(), [](std::pair<std::string, std::shared_ptr<ListenerBase>> p)
                           {
                               auto listener = std::dynamic_pointer_cast<Listener<Args...>>(p.second);

                               if (listener)
                               {
                                   return listener;
                               }
                               else
                               {
                                   throw std::logic_error("EventEmitter::emit: invalid event signature");
                               }
                           });

            for (auto &h : handlers)
            {
                h->callback(args...);
            }
        }

    private:
        struct ListenerBase
        {
            ListenerBase(const std::string &name) : name(name) {}
            virtual ~ListenerBase() {}
            const std::string &name;
        };

        template <typename... Args>
        struct Listener : public ListenerBase
        {
            Listener() {}
            Listener(const std::string &name, std::function<void(Args...)> callback) : ListenerBase(name), callback(callback) {}
            std::function<void(Args...)> callback;
        };

        unsigned int lastListener;
        std::multimap<std::string, std::shared_ptr<ListenerBase>> listeners;

        template <typename T>
        struct function_traits : public function_traits<decltype(&T::operator())>
        {
        };

        template <typename ClassType, typename ReturnType, typename... Args>
        struct function_traits<ReturnType (ClassType::*)(Args...) const>
        {
            typedef std::function<ReturnType(Args...)> f_type;
        };

        template <typename L>
        typename function_traits<L>::f_type make_function(L listener)
        {
            return (typename function_traits<L>::f_type)(listener);
        }
    };
}