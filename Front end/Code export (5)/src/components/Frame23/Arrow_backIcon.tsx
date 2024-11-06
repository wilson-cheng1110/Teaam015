import { memo, SVGProps } from 'react';

const Arrow_backIcon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 168 132' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M40.1625 74.1667L98.9625 119.9L84 131.333L0 66L84 0.666668L98.9625 12.1L40.1625 57.8333H168V74.1667H40.1625Z'
      fill='#1D1B20'
    />
  </svg>
);

const Memo = memo(Arrow_backIcon);
export { Memo as Arrow_backIcon };
