/*
 *      Copyright (C) 2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "LanguageInvokerThread.h"
#include "ScriptInvocationManager.h"
#include "utils/log.h"

CLanguageInvokerThread::CLanguageInvokerThread(ILanguageInvoker *invoker, CScriptInvocationManager *invocationManager)
  : ILanguageInvoker(NULL),
    CThread("LanguageInvoker"),
    m_invoker(invoker),
    m_invocationManager(invocationManager)
{ }

CLanguageInvokerThread::~CLanguageInvokerThread()
{
  printf("debug jc: CLanguageInvokerThread::~CLanguageInvokerThread()\n");
  //Stop(true);
  delete m_invoker;
  printf("debug jc: CLanguageInvokerThread::~CLanguageInvokerThread() terminated\n");
}

InvokerState CLanguageInvokerThread::GetState()
{
  if (m_invoker == NULL)
    return InvokerStateFailed;

  return m_invoker->GetState();
}

bool CLanguageInvokerThread::execute(const std::string &script, const std::vector<std::string> &arguments)
{
  if (m_invoker == NULL || script.empty())
    return false;

  m_script = script;
  m_args = arguments;

  Create();
  return true;
}

bool CLanguageInvokerThread::stop(bool wait)
{
  
  bool myVar;
  
  printf("debug jc: CLanguageInvokerThread::stop\n");
  CLog::Log(LOGNOTICE, "debug jc: CLanguageInvokerThread::stop");
  
  if (m_invoker == NULL)
  {
    printf("debug jc: (m_invoker == NULL) terminating\n");
    return false;
  }

  if (!CThread::IsRunning())
  {
    printf("debug jc: (!CThread::IsRunning()) terminating\n");
    return false;
  }

  bool result = true;
  if (m_invoker->GetState() < InvokerStateDone)
  {
    printf("debug jc: before result = m_invoker->Stop(wait);\n");
    // stop the language-specific invoker
    result = m_invoker->Stop(wait);
    // stop the thread
    printf("debug jc: before CThread::StopThread(wait);\n");
    printf("debug jc: skipping CThread::StopThread(wait);\n");
    //CThread::StopThread(wait);
  }

  printf("debug jc: CLanguageInvokerThread::stop terminating\n");
  CLog::Log(LOGNOTICE, "debug jc: CLanguageInvokerThread::stop terminating");
  return result;
}

void CLanguageInvokerThread::OnStartup()
{
  if (m_invoker == NULL)
    return;

  m_invoker->SetId(GetId());
  if (m_addon != NULL)
    m_invoker->SetAddon(m_addon);
}

void CLanguageInvokerThread::Process()
{
  printf("RetroRig #45: CLanguageInvokerThread::Process() %s \n",m_script.c_str());
  if (m_invoker == NULL)
    return;

  m_invoker->Execute(m_script, m_args);
}

void CLanguageInvokerThread::OnExit()
{
  if (m_invoker == NULL)
    return;

  m_invoker->onExecutionDone();
  m_invocationManager->OnScriptEnded(GetId());
}

void CLanguageInvokerThread::OnException()
{
  if (m_invoker == NULL)
    return;

  m_invoker->onExecutionFailed();
  m_invocationManager->OnScriptEnded(GetId());
}