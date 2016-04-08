/**
 * smssend.cpp
 *
 * SMS sender plugin for YATE Project http://YATE.null.ro
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 ark
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
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <yatephone.h>

using namespace TelEngine;
namespace {

	static const char s_mini[] = "smssend <MSISDN> <CALLER> <message>";
	static const char s_help[] = "send text messages to a particular MS";

	class CmdHandler : public MessageReceiver {
		public:
			enum {
				Status,
				Command,
				Help
			};
			virtual bool received(Message &msg, int id);
			bool doCommand(String& line, String& rval);
			bool doComplete(const String& partLine, const String& partWord, String& rval);
	};

	class SmsSendPlugin : public Plugin {
		public:
			SmsSendPlugin();
			virtual ~SmsSendPlugin();
			virtual void initialize();
			static bool sendSMS(String const &, String const &, String const &);
		private:
			bool m_init = false;
			CmdHandler *m_cmdhandler;
	};

	bool SmsSendPlugin::sendSMS(String const &msisdn, String const &sender, String const &text) {

		Message m("call.route");
		String callto("");

		m.addParam("called", msisdn);
		m.addParam("caller", "yate");
		m.addParam("module", "smssend");
		if (Engine::dispatch(m))
			callto = m.retValue().c_str();
		else {
			Debug(DebugGoOn, "Couldn't retrieve TMSI for MSISDN: %s", msisdn);
			return false;
		}

		m = "msg.execute";
		m.addParam("caller", "12345");
		m.addParam("called", msisdn);
		m.addParam("text", text);
		m.addParam("sms.caller", sender);
		m.addParam("callto", callto);

		if (!Engine::dispatch(m)) {
			Debug(DebugGoOn, "Failed to send SMS.");
			return false;
		}
		return true;
	}

	bool CmdHandler::doComplete(const String& partLine, const String& partWord, String& rval) {
		// For autocompletion purpose.
		return false;
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	bool CmdHandler::received(Message &msg, int id) {
		String tmp;

		switch (id) {
			case Status:
				break;
			case Command:
				tmp = msg.getValue(YSTRING("line"));
				if (tmp.startSkip("smssend")) {
					int s = tmp.find(' ');

					if (s >= 0)	{

						String msisdn = tmp.substr(0, s);
						String caller;

						tmp.startSkip(msisdn);
						tmp = tmp.trimBlanks();
						s = tmp.find(' ');
						if (s >= 0) {
							caller = tmp.substr(0, s);
							// Sending text message
							return SmsSendPlugin::sendSMS(msisdn, caller, tmp.substr(s + 1));
						}
					}
				}
				break;
			case Help:
				tmp = msg.getValue(YSTRING("line"));
				if (tmp.null() || (tmp == YSTRING("smssend"))) {
					msg.retValue() << "  " << s_mini << "\r\n";
					if (tmp) {
						msg.retValue() << s_help << "\r\n";
						return true;
					}
				}
				break;
		}
		return false;
	}

	SmsSendPlugin::SmsSendPlugin()
		: Plugin("smssend"), m_init(false), m_cmdhandler(NULL) {
			Output("Loaded module Sms Sender");
		}

	SmsSendPlugin::~SmsSendPlugin() {
		Output("Unloading module Sms Sender.");
	}

	void SmsSendPlugin::initialize() {

		Output("Initializing module Sms Sender");
		if(!m_init) {
			m_cmdhandler = new CmdHandler;
			Engine::install(new MessageRelay("engine.status", m_cmdhandler, CmdHandler::Status, 100, name()));
			Engine::install(new MessageRelay("engine.command", m_cmdhandler, CmdHandler::Command, 100, name()));
			Engine::install(new MessageRelay("engine.help", m_cmdhandler, CmdHandler::Help, 100, name()));
			m_init = true;
		}
	}

	INIT_PLUGIN(SmsSendPlugin);

};
