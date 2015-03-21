// actor_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "ios_proxy.h"
#include "actor_framework.h"
#include "msg_pipe.h"
#include "async_buffer.h"
#include "time_info.h"
#include <list>
#include <Windows.h>

using namespace std;

void check_key_down(my_actor* self, int id)
{//��ⰴ������
	while (GetAsyncKeyState(id) >= 0)
	{
		self->sleep(1);
	}
}

void check_key_up(my_actor* self, int id)
{//��ⰴ������
	while (GetAsyncKeyState(id) < 0)
	{
		self->sleep(1);
	}
}

void check_key_test(my_actor* self, int id)
{
	while (true)
	{//�������º󣬼�ⵯ��1000ms��û�е�����ǳ�ʱ����
		check_key_down(self, id);//��ⰴ��
		child_actor_handle checkUp = self->create_child_actor(boost::bind(&check_key_up, _1, id), 24 kB);//����һ����ⵯ�����Actor
		self->child_actor_run(checkUp);//��ʼ������Actor
		self->delay_trig(1000, boost::bind(&my_actor::notify_quit, checkUp.get_actor()));//���õ���ʱ��������ʱ��ǿ�ƹر�checkUp
		if (self->child_actor_wait_quit(checkUp))//�����˳��ķ���true����ǿ�ƹرյķ���false
		{
			self->cancel_delay_trig();
			printf("ok %d\n", id);
		} 
		else
		{//����ʱ
			printf("timeout %d\n", id);
			check_key_up(self, id);//��ⵯ��
		}
	}
}

void actor_print(my_actor* self, int id)
{//���º��ȴ�ӡһ���ַ���500ms��ÿ��50ms��ӡһ��
	printf("_");
	self->sleep(500);
	while (true)
	{
		printf("-");
		self->sleep(50);
	}
}

void actor_test_print(my_actor* self, int id)
{
	while (true)
	{
		check_key_down(self, id);//��ⰴ��
		child_actor_handle ch = self->create_child_actor(boost::bind(&actor_print, _1, id));
		self->child_actor_run(ch);
		check_key_up(self, id);//��ⵯ��
		self->child_actor_force_quit(ch);
	}
}

void actor_suspend(my_actor* self, const list<actor_handle>& chs)
{
	while (true)
	{
		check_key_down(self, VK_RBUTTON);
		self->actors_suspend(chs);
		check_key_up(self, VK_RBUTTON);
	}
}

void actor_resume(my_actor* self, const list<actor_handle>& chs)
{
	while (true)
	{
		check_key_down(self, VK_LBUTTON);
		self->actors_resume(chs);
		check_key_up(self, VK_LBUTTON);
	}
}

void check_both_up(my_actor* self, int id, bool& own, const bool& another)
{
	goto __start;
	while (!another)
	{
		self->sleep(1);
		own = false;
__start:
		check_key_up(self, id);
		own = true;
	}
}

void check_two_down(my_actor* self, int dt, int id1, int id2)
{
	while (true)
	{
		{//������������Ƿ񶼴��ڵ���״̬
			bool st1 = false;
			bool st2 = false;
			child_actor_handle up1 = self->create_child_actor(boost::bind(&check_both_up, _1, id1, boost::ref(st1), boost::ref(st2)));
			child_actor_handle up2 = self->create_child_actor(boost::bind(&check_both_up, _1, id2, boost::ref(st2), boost::ref(st1)));
			async_trig_handle<bool> ath;
			auto nh = self->begin_trig(ath);
			up1.get_actor()->append_quit_callback(nh);
			up2.get_actor()->append_quit_callback(nh);
			self->child_actor_run(up1);
			self->child_actor_run(up2);
			if (self->timed_wait_trig(ath, st1, 3000))
			{
				self->child_actor_force_quit(up1);
				self->child_actor_force_quit(up2);
			}
			else
			{
				printf("�˳� check_two_down\n");
				self->child_actor_force_quit(up1);
				self->child_actor_force_quit(up2);
				self->close_trig(ath);
				break;
			}
		}
		{
			child_actor_handle check1 = self->create_child_actor(boost::bind(&check_key_down, _1, id1));
			child_actor_handle check2 = self->create_child_actor(boost::bind(&check_key_down, _1, id2));
			async_trig_handle<actor_handle, bool> ath;
			auto nh = self->begin_trig(ath);
			check1.get_actor()->append_quit_callback(boost::bind(nh, check2.get_actor(), _1));
			check2.get_actor()->append_quit_callback(boost::bind(nh, check1.get_actor(), _1));
			self->child_actor_run(check1);
			self->child_actor_run(check2);
			actor_handle ah;
			bool ok = false;
			self->wait_trig(ath, ah, ok);
			self->delay_trig(dt, boost::bind(&my_actor::notify_quit, ah));
			if (self->actor_wait_quit(ah))
			{
				printf("*success*\n");
			} 
			else
			{
				printf("*failure*\n");
			}
			self->cancel_delay_trig();
			self->child_actor_force_quit(check1);
			self->child_actor_force_quit(check2);
		}
	}
}

void wait_key(my_actor* self, int id, boost::function<void (int)> cb)
{
	check_key_up(self, id);
	while (true)
	{
		check_key_down(self, id);
		cb(id);
		check_key_up(self, id);
	}
}

void shift_key(my_actor* self, actor_handle pauseactor)
{
	list<child_actor_handle::ptr> childs;
	actor_msg_handle<int> amh;
	auto h = self->make_msg_notify(amh);
	for (int i = 'A'; i <= 'Z'; i++)
	{
		auto tp = child_actor_handle::make_ptr();
		*tp = self->create_child_actor(boost::bind(&wait_key, _1, i, h));
		self->child_actor_run(*tp);
		childs.push_back(tp);
	}
	while (true)
	{
		int id = self->pump_msg(amh);
		if ('P' == id)
		{
			bool isPause = self->actor_switch(pauseactor);
			printf("%s���ܲ���\n", isPause? "��ͣ": "�ָ�");
		}
		else
		{
			printf("shift+%c\n", id);
		}
	}
	self->close_msg_notify(amh);
	self->child_actors_force_quit(childs);
}

void test_shift(my_actor* self, actor_handle pauseactor)
{
	child_actor_handle ch;
	while (true)
	{
		check_key_down(self, VK_SHIFT);
		ch = self->create_child_actor(boost::bind(&shift_key, _1, pauseactor));
		self->child_actor_run(ch);
		check_key_up(self, VK_SHIFT);
		self->child_actor_force_quit(ch);
	}
}

void count_test(my_actor* self, int& ct)
{
	while (true)
	{
		ct++;
		self->sleep(0);
	}
}

void perfor_test(my_actor* self, ios_proxy& ios, int actorNum)
{
	int* count = (int*)_alloca(actorNum*sizeof(int));
	self->check_stack();
	vector<shared_strand> strands;
	strands.resize(ios.threadNumber());
	for (int i = 0; i < (int)ios.threadNumber(); i++)
	{
		strands[i] = boost_strand::create(ios);
	}

	for (int num = 1; true; num = num % actorNum + 1)
	{
		long long tk = get_tick_us();
		list<child_actor_handle::ptr> childList;
		for (int i = 0; i < num; i++)
		{
			count[i] = 0;
			auto newactor = child_actor_handle::make_ptr();
			*newactor = self->create_child_actor(strands[i%strands.size()], boost::bind(&count_test, _1, boost::ref(count[i])));
			childList.push_front(newactor);
			self->child_actor_run(*childList.front());
		}
		self->sleep(1000);
		self->child_actors_force_quit(childList);
		int ct = 0;
		for (int i = 0; i < num; i++)
		{
			ct += count[i];
		}
		double f = (double)ct * 1000000 / (get_tick_us()-tk);
		printf("Actor��=%d, �л�Ƶ��=%d\n", num, (int)f);
	}
}

void create_null_actor(my_actor* self, int* count)
{
	while (true)
	{
		size_t yieldTick = self->yield_count();
		list<child_actor_handle::ptr> childList;
		for (int i = 0; i < 100; i++)
		{
			(*count)++;
			auto newactor = child_actor_handle::make_ptr();
			*newactor = self->create_child_actor([](my_actor*){});
			childList.push_back(newactor);
		}
		self->child_actors_force_quit(childList);
		if (self->yield_count() == yieldTick)
		{
			self->sleep(0);
		}
	}
}

void create_actor_test(my_actor* self)
{
	while (true)
	{
		int count = 0;
		child_actor_handle createactor = self->create_child_actor(boost::bind(&create_null_actor, _1, &count));
		self->child_actor_run(createactor);
		self->sleep(1000);
		self->child_actor_force_quit(createactor);
		printf("1�봴��Actor��=%d\n", count);
	}
}

void async_buffer_read(my_actor* self, boost::shared_ptr<async_buffer<int> > buffer, msg_pipe<>::regist_reader regWaitFull, int max)
{
	actor_msg_handle<> amh;
	regWaitFull(self, amh);
	goto __start;
	while (true)
	{
		self->sleep(500);
		int i;
		bool empty = buffer->pop(i);
		printf("%d\n", i);
		if (max == i)
		{
			break;
		}
		if (empty)
		{
			printf("read �����\n");
			__start:
			self->pump_msg(amh);
			printf("read ����������\n");
		}
	}
	self->close_msg_notify(amh);
}

void async_buffer_test(my_actor* self)
{
	msg_pipe<>::writer_type fullNotify;
	msg_pipe<>::regist_reader regWaitFull = msg_pipe<>::make(fullNotify);
	boost::shared_ptr<async_buffer<int> > buffer = async_buffer<int>::create(10);
	actor_msg_handle<> amh;
	int testCyc = 30;
	buffer->setNotify(fullNotify, self->make_msg_notify(amh));
	child_actor_handle readactor = self->create_child_actor(boost::bind(&async_buffer_read, _1, buffer, regWaitFull, testCyc-1));
	self->child_actor_run(readactor);
	for (int i = 0; i < testCyc; i++)
	{
		check_key_down(self, VK_CONTROL);
		check_key_up(self, VK_CONTROL);
		if (buffer->push(i))
		{
			printf("writer ������\n");
			self->pump_msg(amh);
			printf("writer ������\n");
		}
	}
	self->child_actor_wait_quit(readactor);
	self->close_msg_notify(amh);
	printf("������Խ���\n");
}

void actor_test(my_actor* self)
{
	ios_proxy perforIos;//���ڲ��Զ��߳��µ�Actor�л�����
	perforIos.run(ios_proxy::hardwareConcurrency());//�������߳�����ΪCPU�߳���
	perforIos.runPriority(ios_proxy::idle);//���������ȼ�����Ϊ���
	child_actor_handle actorLeft = self->create_child_actor(boost::bind(&check_key_test, _1, VK_LEFT));
	child_actor_handle actorRight = self->create_child_actor(boost::bind(&check_key_test, _1, VK_RIGHT));
	child_actor_handle actorUp = self->create_child_actor(boost::bind(&check_key_test, _1, VK_UP));
	child_actor_handle actorDown = self->create_child_actor(boost::bind(&check_key_test, _1, VK_DOWN));
	child_actor_handle actorPrint = self->create_child_actor(boost::bind(&actor_test_print, _1, VK_SPACE));//���ո��
	child_actor_handle actorTwo = self->create_child_actor(boost::bind(&check_two_down, _1, 10, 'D', 'F'));//���D,F���Ƿ�ͬʱ����(���ü�����10ms)
	child_actor_handle actorPerfor = self->create_child_actor(boost::bind(&perfor_test, _1, boost::ref(perforIos), 200));//Actor�л����ܲ���
	child_actor_handle actorCreate = self->create_child_actor(boost::bind(&create_actor_test, _1));//Actor�������ܲ���
	child_actor_handle actorShift = self->create_child_actor(boost::bind(&test_shift, _1, actorPerfor.get_actor()));//shift+��ĸ���
	child_actor_handle actorBuffer = self->create_child_actor(boost::bind(&async_buffer_test, _1));//�첽������в���
	child_actor_handle actorProducer1;
	child_actor_handle actorProducer2;
	child_actor_handle actorConsumer;
	//����������/������ģ�Ͳ���
	actor_msg_handle<int, int> conCmh;
	{
		actorConsumer = self->create_child_actor([&conCmh](my_actor* self)
		{//������
			while (true)
			{
				int p0;
				int id;
				self->pump_msg(conCmh, p0, id);
				printf("%d-%d id=%d\n", p0, (int)conCmh.length(), id);
				self->sleep(1000);
			}
			self->close_msg_notify(conCmh);
		});
		auto writer = actorConsumer.get_actor()->make_msg_notify(conCmh);
		auto test_producer = [writer](my_actor* self)
		{//������
			for (int i = 0; true; i++)
			{
				writer(i, (int)self->this_id());
				self->sleep(2100);
			}
		};
		actorProducer1 = self->create_child_actor(test_producer);
		actorProducer2 = self->create_child_actor(test_producer);
	}

	list<actor_handle> chs;//��Ҫ�������Actor���󣬿��Դ��·�ע�ͼ�������
	chs.push_back(actorLeft.get_actor());
	chs.push_back(actorRight.get_actor());
	chs.push_back(actorUp.get_actor());
	chs.push_back(actorDown.get_actor());
	chs.push_back(actorPrint.get_actor());
	chs.push_back(actorShift.get_actor());
	chs.push_back(actorTwo.get_actor());
	chs.push_back(actorConsumer.get_actor());
//	chs.push_back(actorPerfor.get_actor());
	chs.push_back(actorProducer1.get_actor());
	chs.push_back(actorProducer2.get_actor());
	chs.push_back(actorCreate.get_actor());
	child_actor_handle actorSuspend = self->create_child_actor(boost::bind(&actor_suspend, _1, boost::ref(chs)), 32 kB);//�������Ҽ���ͣ�������
	child_actor_handle actorResume = self->create_child_actor(boost::bind(&actor_resume, _1, boost::ref(chs)), 32 kB);//����������ָ��������
	self->child_actor_run(actorLeft);
	self->child_actor_run(actorRight);
	self->child_actor_run(actorUp);
	self->child_actor_run(actorDown);
	self->child_actor_run(actorPrint);
	self->child_actor_run(actorShift);
	self->child_actor_run(actorTwo);
	self->child_actor_run(actorPerfor);
// 	self->child_actor_run(actorConsumer);
// 	self->child_actor_run(actorProducer1);
// 	self->child_actor_run(actorProducer2);
//	self->child_actor_run(actorCreate);
	self->child_actor_run(actorSuspend);
	self->child_actor_run(actorResume);
	self->child_actor_run(actorBuffer);
	self->child_actor_suspend(actorPerfor);
	check_key_down(self, VK_ESCAPE);//ESC���˳�
	self->child_actor_force_quit(actorLeft);
	self->child_actor_force_quit(actorRight);
	self->child_actor_force_quit(actorUp);
	self->child_actor_force_quit(actorDown);
	self->child_actor_force_quit(actorPrint);
	self->child_actor_force_quit(actorShift);
	self->child_actor_force_quit(actorTwo);
	self->child_actor_force_quit(actorPerfor);
	self->child_actor_force_quit(actorConsumer);
	self->child_actor_force_quit(actorProducer1);
	self->child_actor_force_quit(actorProducer2);
	self->child_actor_force_quit(actorCreate);
	self->child_actor_force_quit(actorSuspend);
	self->child_actor_force_quit(actorResume);
	self->child_actor_force_quit(actorBuffer);
	perforIos.stop();
}


	/*
	�߼����Ʋ��Գ���
	�������ҷ������⣬���º�1000ms�ڵ����ӡok�������ӡtimeout;
	�ո����⣬���º��ӡ _ ��500ms��û�е���ÿ��50ms��ӡ - ;
	D,F����⣬10ms��������"ͬʱ"����(����"ͬʱ"��������һǰһ��)��ӡsuccess�������ӡfailure;
	��갴����⣬�Ҽ����¹�������������»ָ�����;
	������/������ģ�Ͳ���;
	ESC���˳�;
	ע�⣺ĳЩ���̿��ܲ�֧��2�����ϰ���ͬʱ����
	*/
int main(int argc, char* argv[])
{
	enable_high_resolution();
	my_actor::enable_stack_pool();
	ios_proxy ios;
	ios.run();
	{
		actor_handle actorTest = my_actor::create(boost_strand::create(ios), boost::bind(&actor_test, _1));
		actorTest->notify_run();
		actorTest->outside_wait_quit();
	}
 	ios.stop();
	return 0;
}