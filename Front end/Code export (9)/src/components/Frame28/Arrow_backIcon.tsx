import { memo, SVGProps } from 'react';

const Arrow_backIcon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 152 142' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M36.3375 79.8333L89.5375 129.3L76 141.667L0 71L76 0.333332L89.5375 12.7L36.3375 62.1667H152V79.8333H36.3375Z'
      fill='#1D1B20'
    />
  </svg>
);

const Memo = memo(Arrow_backIcon);
export { Memo as Arrow_backIcon };
