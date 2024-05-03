# include <nuttx/sched_note.h>
# include <nuttx/itm/itm.h>


void sched_note_start(struct tcb_s *tcb)
{
  EMDBG_LOG_TASK_START(tcb);
}

void sched_note_stop(struct tcb_s *tcb)
{
  EMDBG_LOG_TASK_STOP(tcb);
}

static int emdbg_prev_state = 0;
void sched_note_suspend(struct tcb_s *tcb)
{
  emdbg_prev_state = tcb->task_state;
}

void sched_note_resume(struct tcb_s *tcb)
{
  EMDBG_LOG_TASK_RESUME(tcb, emdbg_prev_state);
}