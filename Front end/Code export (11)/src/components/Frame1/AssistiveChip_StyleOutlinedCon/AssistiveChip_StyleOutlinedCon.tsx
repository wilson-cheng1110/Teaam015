import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './AssistiveChip_StyleOutlinedCon.module.css';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
  text?: {
    labelText?: ReactNode;
  };
}
/* @figmaId 158:204 */
export const AssistiveChip_StyleOutlinedCon: FC<Props> = memo(function AssistiveChip_StyleOutlinedCon(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.stateLayer}>
        {props.text?.labelText != null ? props.text?.labelText : <div className={classes.labelText}>Label</div>}
      </div>
    </div>
  );
});
